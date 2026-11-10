#include "nunlauncher/boot.h"

#include <stddef.h>

static int architecture_known(uint32_t architecture)
{
    return architecture == NUNLAUNCHER_ARCH_X86_64 ||
           architecture == NUNLAUNCHER_ARCH_AARCH64 ||
           architecture == NUNLAUNCHER_ARCH_RISCV64 ||
           architecture == NUNLAUNCHER_ARCH_E2K;
}

static int memory_type_known(uint32_t type)
{
    return type == NUNLAUNCHER_MEMORY_AVAILABLE ||
           type == NUNLAUNCHER_MEMORY_LOADED_KERNEL ||
           type == NUNLAUNCHER_MEMORY_RESERVED ||
           type == NUNLAUNCHER_MEMORY_FRAMEBUFFER;
}

static int range_end(uint64_t base, uint64_t length, uint64_t *end_out)
{
    if (length == 0U || end_out == NULL || UINT64_MAX - base < length) {
        return 0;
    }
    *end_out = base + length;
    return 1;
}

nunlauncher_status_t nunlauncher_image_validate(
    const nunlauncher_image_descriptor_t *image,
    size_t available_bytes,
    uint32_t selected_architecture)
{
    uint64_t image_end;

    if (image == NULL) {
        return NUNLAUNCHER_EINVAL;
    }
    if (image->magic != NUNLAUNCHER_IMAGE_MAGIC ||
        image->version != NUNLAUNCHER_IMAGE_VERSION ||
        image->header_size < (uint32_t)sizeof(*image) ||
        image->flags != 0U || image->reserved != 0U ||
        image->image_size == 0U) {
        return NUNLAUNCHER_EIMAGE;
    }
    if (!architecture_known(selected_architecture) ||
        image->architecture != selected_architecture) {
        return NUNLAUNCHER_EARCH;
    }
    if (image->image_size > (uint64_t)available_bytes ||
        image->entry_offset >= image->image_size ||
        !range_end(image->load_address, image->image_size, &image_end)) {
        return NUNLAUNCHER_ERANGE;
    }
    (void)image_end;
    return NUNLAUNCHER_OK;
}

nunlauncher_status_t nunlauncher_boot_info_validate(
    const nunlauncher_boot_info_t *info)
{
    uint64_t kernel_end;
    uint32_t i;
    uint32_t kernel_regions = 0U;
    int entry_in_kernel_region = 0;

    if (info == NULL) {
        return NUNLAUNCHER_EINVAL;
    }
    if (info->magic != NUNLAUNCHER_BOOT_INFO_MAGIC ||
        info->version != NUNLAUNCHER_BOOT_INFO_VERSION ||
        info->struct_size < (uint32_t)sizeof(*info) ||
        info->flags != 0U) {
        return NUNLAUNCHER_EVERSION;
    }
    if (!architecture_known(info->architecture)) {
        return NUNLAUNCHER_EARCH;
    }
    if (info->memory_regions == NULL ||
        info->memory_region_count == 0U ||
        info->memory_region_count > NUNLAUNCHER_MAX_MEMORY_REGIONS) {
        return NUNLAUNCHER_EMEMMAP;
    }
    if (!range_end(info->kernel_base, info->kernel_size, &kernel_end)) {
        return NUNLAUNCHER_ERANGE;
    }
    if (info->entry_address < info->kernel_base ||
        info->entry_address >= kernel_end) {
        return NUNLAUNCHER_EENTRY;
    }

    for (i = 0U; i < info->memory_region_count; ++i) {
        const nunlauncher_memory_region_t *region = &info->memory_regions[i];
        uint64_t region_end;
        uint32_t j;

        if (region->reserved != 0U || !memory_type_known(region->type) ||
            !range_end(region->base, region->length, &region_end)) {
            return NUNLAUNCHER_EMEMMAP;
        }
        if (region->type == NUNLAUNCHER_MEMORY_LOADED_KERNEL &&
            region->base <= info->kernel_base &&
            region_end >= kernel_end) {
            ++kernel_regions;
        }
        if (region->type == NUNLAUNCHER_MEMORY_LOADED_KERNEL &&
            info->entry_address >= region->base &&
            info->entry_address < region_end) {
            entry_in_kernel_region = 1;
        }
        for (j = i + 1U; j < info->memory_region_count; ++j) {
            const nunlauncher_memory_region_t *other = &info->memory_regions[j];
            uint64_t other_end;
            if (other->reserved != 0U || !memory_type_known(other->type) ||
                !range_end(other->base, other->length, &other_end)) {
                return NUNLAUNCHER_EMEMMAP;
            }
            if (region->base < other_end && other->base < region_end) {
                return NUNLAUNCHER_EOVERLAP;
            }
        }
    }

    if (kernel_regions != 1U || !entry_in_kernel_region) {
        return NUNLAUNCHER_EENTRY;
    }
    return NUNLAUNCHER_OK;
}

nunlauncher_status_t nunlauncher_handoff_prepare(
    const nunlauncher_image_descriptor_t *image,
    size_t available_bytes,
    const nunlauncher_boot_info_t *candidate,
    nunlauncher_boot_info_t *handoff_out)
{
    nunlauncher_status_t status;

    if (candidate == NULL || handoff_out == NULL) {
        return NUNLAUNCHER_EINVAL;
    }
    status = nunlauncher_image_validate(
        image, available_bytes, candidate->architecture);
    if (status != NUNLAUNCHER_OK) {
        return status;
    }
    status = nunlauncher_boot_info_validate(candidate);
    if (status != NUNLAUNCHER_OK) {
        return status;
    }
    if (image->load_address != candidate->kernel_base ||
        image->image_size != candidate->kernel_size ||
        image->load_address + image->entry_offset != candidate->entry_address) {
        return NUNLAUNCHER_EMISMATCH;
    }

    *handoff_out = *candidate;
    return NUNLAUNCHER_OK;
}
