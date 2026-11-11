#include "nunlauncher/boot_contract.h"

#include <string.h>

#define NUNL_CRC_OFFSET 40u
#define NUNL_CRC_SIZE 4u
#define NUNL_VERSION 1u

static uint32_t read_le32(const uint8_t *bytes)
{
    return (uint32_t)bytes[0] |
           ((uint32_t)bytes[1] << 8) |
           ((uint32_t)bytes[2] << 16) |
           ((uint32_t)bytes[3] << 24);
}

static uint64_t read_le64(const uint8_t *bytes)
{
    return (uint64_t)read_le32(bytes) |
           ((uint64_t)read_le32(bytes + 4) << 32);
}

nunl_result_t nunl_validate_memory_map(const nunl_memory_range_t *memory_map,
                                       size_t memory_map_count)
{
    size_t i, j;
    if (memory_map == NULL || memory_map_count == 0u ||
        memory_map_count > NUNL_MAX_MEMORY_RANGES) {
        return NUNL_EINVAL;
    }
    for (i = 0u; i < memory_map_count; ++i) {
        const nunl_memory_range_t *left = &memory_map[i];
        uint64_t left_end;
        if (left->size == 0u || left->base > UINT64_MAX - left->size ||
            left->reserved != 0u ||
            left->type < (uint32_t)NUNL_MEMORY_USABLE_RAM ||
            left->type > (uint32_t)NUNL_MEMORY_MMIO) {
            return NUNL_EMEMORY;
        }
        left_end = left->base + left->size;
        for (j = i + 1u; j < memory_map_count; ++j) {
            const nunl_memory_range_t *right = &memory_map[j];
            uint64_t right_end;
            if (right->size == 0u || right->base > UINT64_MAX - right->size ||
                right->reserved != 0u ||
                right->type < (uint32_t)NUNL_MEMORY_USABLE_RAM ||
                right->type > (uint32_t)NUNL_MEMORY_MMIO) {
                return NUNL_EMEMORY;
            }
            right_end = right->base + right->size;
            if (left->base < right_end && right->base < left_end) {
                return NUNL_EOVERLAP;
            }
        }
    }
    return NUNL_OK;
}

uint32_t nunl_image_header_crc32(const void *header_bytes, size_t header_length)
{
    const uint8_t *bytes = (const uint8_t *)header_bytes;
    uint32_t crc = UINT32_C(0xFFFFFFFF);
    size_t i;
    unsigned bit;

    if (bytes == NULL || header_length < NUNL_IMAGE_HEADER_SIZE) {
        return 0u;
    }

    for (i = 0u; i < NUNL_IMAGE_HEADER_SIZE; ++i) {
        uint8_t value = (i >= NUNL_CRC_OFFSET && i < (NUNL_CRC_OFFSET + NUNL_CRC_SIZE))
                            ? 0u
                            : bytes[i];
        crc ^= (uint32_t)value;
        for (bit = 0u; bit < 8u; ++bit) {
            const uint32_t mask = (uint32_t)-(int32_t)(crc & 1u);
            crc = (crc >> 1) ^ (UINT32_C(0xEDB88320) & mask);
        }
    }
    return ~crc;
}

nunl_result_t nunl_validate_image(const void *image,
                                  size_t available_bytes,
                                  uint64_t load_address,
                                  const nunl_memory_range_t *memory_map,
                                  size_t memory_map_count,
                                  nunl_architecture_t target_architecture,
                                  uint64_t *entry_address_out)
{
    const uint8_t *bytes = (const uint8_t *)image;
    uint32_t version;
    uint32_t header_size;
    uint64_t image_size;
    uint64_t entry_offset;
    uint32_t image_architecture;
    uint32_t flags;
    uint32_t header_crc;
    uint32_t reserved;
    uint64_t entry_address;
    size_t i;
    int contained = 0;

    nunl_result_t map_result;
    if (bytes == NULL || memory_map == NULL || memory_map_count == 0u ||
        memory_map_count > NUNL_MAX_MEMORY_RANGES || entry_address_out == NULL ||
        target_architecture == NUNL_ARCH_UNKNOWN) {
        return NUNL_EINVAL;
    }
    map_result = nunl_validate_memory_map(memory_map, memory_map_count);
    if (map_result != NUNL_OK) {
        return map_result;
    }
    if (available_bytes < NUNL_IMAGE_HEADER_SIZE) {
        return NUNL_ESIZE;
    }
    if (memcmp(bytes, NUNL_IMAGE_MAGIC, NUNL_IMAGE_MAGIC_SIZE) != 0) {
        return NUNL_EFORMAT;
    }

    version = read_le32(bytes + 8u);
    header_size = read_le32(bytes + 12u);
    image_size = read_le64(bytes + 16u);
    entry_offset = read_le64(bytes + 24u);
    image_architecture = read_le32(bytes + 32u);
    flags = read_le32(bytes + 36u);
    header_crc = read_le32(bytes + NUNL_CRC_OFFSET);
    reserved = read_le32(bytes + 44u);

    if (version != NUNL_VERSION || header_size != NUNL_IMAGE_HEADER_SIZE) {
        return NUNL_EVERSION;
    }
    if (image_architecture < (uint32_t)NUNL_ARCH_X86 ||
        image_architecture > (uint32_t)NUNL_ARCH_E2K ||
        image_architecture != (uint32_t)target_architecture) {
        return NUNL_EARCH;
    }
    if (flags != 0u || reserved != 0u) {
        return NUNL_EFORMAT;
    }
    if (image_size < (uint64_t)header_size || image_size > (uint64_t)available_bytes) {
        return NUNL_ESIZE;
    }
    if (entry_offset < (uint64_t)header_size || entry_offset >= image_size ||
        load_address > UINT64_MAX - entry_offset) {
        return NUNL_EENTRY;
    }
    if ((load_address % NUNL_BOOT_PAGE_SIZE) != 0u) {
        return NUNL_EMEMORY;
    }
    if (header_crc != nunl_image_header_crc32(bytes, available_bytes)) {
        return NUNL_ECHECKSUM;
    }

    for (i = 0u; i < memory_map_count; ++i) {
        const nunl_memory_range_t *range = &memory_map[i];
        uint64_t range_end;

        range_end = range->base + range->size;
        if (range->type == (uint32_t)NUNL_MEMORY_USABLE_RAM &&
            load_address >= range->base && load_address < range_end &&
            image_size <= (range_end - load_address)) {
            contained = 1;
        }
    }
    if (!contained) {
        return NUNL_EMEMORY;
    }

    entry_address = load_address + entry_offset;
    *entry_address_out = entry_address;
    return NUNL_OK;
}

const char *nunl_result_name(nunl_result_t result)
{
    switch (result) {
        case NUNL_OK: return "ok";
        case NUNL_EINVAL: return "invalid_argument";
        case NUNL_EFORMAT: return "invalid_format";
        case NUNL_EVERSION: return "unsupported_version";
        case NUNL_EARCH: return "architecture_mismatch";
        case NUNL_ESIZE: return "invalid_size";
        case NUNL_EENTRY: return "invalid_entry";
        case NUNL_EMEMORY: return "invalid_memory_range";
        case NUNL_ECHECKSUM: return "header_checksum_mismatch";
        case NUNL_EOVERLAP: return "overlapping_memory_ranges";
        default: return "unknown_result";
    }
}
