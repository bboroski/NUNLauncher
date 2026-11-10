#ifndef NUNLAUNCHER_BOOT_H
#define NUNLAUNCHER_BOOT_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NUNLAUNCHER_BOOT_INFO_MAGIC UINT32_C(0x4E554E42)
#define NUNLAUNCHER_BOOT_INFO_VERSION 1U
#define NUNLAUNCHER_IMAGE_MAGIC UINT32_C(0x4E554E49)
#define NUNLAUNCHER_IMAGE_VERSION 1U
#define NUNLAUNCHER_MAX_MEMORY_REGIONS 64U

typedef enum {
    NUNLAUNCHER_ARCH_UNKNOWN = 0,
    NUNLAUNCHER_ARCH_X86_64 = 1,
    NUNLAUNCHER_ARCH_AARCH64 = 2,
    NUNLAUNCHER_ARCH_RISCV64 = 3,
    /* Identifier reserved for explicit Elbrus/E2K profiles; not a port claim. */
    NUNLAUNCHER_ARCH_E2K = 4
} nunlauncher_architecture_t;

typedef enum {
    NUNLAUNCHER_MEMORY_AVAILABLE = 1,
    NUNLAUNCHER_MEMORY_LOADED_KERNEL = 2,
    NUNLAUNCHER_MEMORY_RESERVED = 3,
    NUNLAUNCHER_MEMORY_FRAMEBUFFER = 4
} nunlauncher_memory_type_t;

typedef struct {
    uint64_t base;
    uint64_t length;
    uint32_t type;
    uint32_t reserved;
} nunlauncher_memory_region_t;

typedef struct {
    uint32_t magic;
    uint32_t version;
    uint32_t struct_size;
    uint32_t architecture;
    uint64_t kernel_base;
    uint64_t kernel_size;
    uint64_t entry_address;
    const nunlauncher_memory_region_t *memory_regions;
    uint32_t memory_region_count;
    uint32_t flags;
} nunlauncher_boot_info_t;

typedef struct {
    uint32_t magic;
    uint32_t version;
    uint32_t header_size;
    uint32_t architecture;
    uint64_t image_size;
    uint64_t load_address;
    uint64_t entry_offset;
    uint32_t flags;
    uint32_t reserved;
} nunlauncher_image_descriptor_t;

typedef enum {
    NUNLAUNCHER_OK = 0,
    NUNLAUNCHER_EINVAL = -1,
    NUNLAUNCHER_EVERSION = -2,
    NUNLAUNCHER_EARCH = -3,
    NUNLAUNCHER_EMEMMAP = -4,
    NUNLAUNCHER_ERANGE = -5,
    NUNLAUNCHER_EOVERLAP = -6,
    NUNLAUNCHER_EENTRY = -7,
    NUNLAUNCHER_EIMAGE = -8,
    NUNLAUNCHER_EMISMATCH = -9
} nunlauncher_status_t;

nunlauncher_status_t nunlauncher_image_validate(
    const nunlauncher_image_descriptor_t *image,
    size_t available_bytes,
    uint32_t selected_architecture);

nunlauncher_status_t nunlauncher_boot_info_validate(
    const nunlauncher_boot_info_t *info);

/*
 * Validate image and handoff metadata and copy the validated descriptor.
 * This function does not perform a CPU mode switch or jump to the entry point.
 */
nunlauncher_status_t nunlauncher_handoff_prepare(
    const nunlauncher_image_descriptor_t *image,
    size_t available_bytes,
    const nunlauncher_boot_info_t *candidate,
    nunlauncher_boot_info_t *handoff_out);

#ifdef __cplusplus
}
#endif

#endif
