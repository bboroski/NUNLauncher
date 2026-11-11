#ifndef NUNLAUNCHER_BOOT_CONTRACT_H
#define NUNLAUNCHER_BOOT_CONTRACT_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define NUNL_IMAGE_HEADER_SIZE 48u
#define NUNL_IMAGE_MAGIC_SIZE 8u
#define NUNL_IMAGE_MAGIC "NUNIMG1\0"
#define NUNL_BOOT_PAGE_SIZE UINT64_C(4096)
#define NUNL_MAX_MEMORY_RANGES 32u

typedef enum nunl_architecture {
    NUNL_ARCH_UNKNOWN = 0,
    NUNL_ARCH_X86 = 1,
    NUNL_ARCH_X86_64 = 2,
    NUNL_ARCH_ARM32 = 3,
    NUNL_ARCH_AARCH64 = 4,
    NUNL_ARCH_RISCV32 = 5,
    NUNL_ARCH_RISCV64 = 6,
    NUNL_ARCH_E2K = 7
} nunl_architecture_t;

typedef enum nunl_memory_type {
    NUNL_MEMORY_USABLE_RAM = 1,
    NUNL_MEMORY_RESERVED = 2,
    NUNL_MEMORY_MMIO = 3
} nunl_memory_type_t;

typedef struct nunl_memory_range {
    uint64_t base;
    uint64_t size;
    uint32_t type;
    uint32_t reserved;
} nunl_memory_range_t;

typedef enum nunl_result {
    NUNL_OK = 0,
    NUNL_EINVAL = -1,
    NUNL_EFORMAT = -2,
    NUNL_EVERSION = -3,
    NUNL_EARCH = -4,
    NUNL_ESIZE = -5,
    NUNL_EENTRY = -6,
    NUNL_EMEMORY = -7,
    NUNL_ECHECKSUM = -8,
    NUNL_EOVERLAP = -9
} nunl_result_t;

/* Validate bounded, non-overlapping memory ranges. Adjacent ranges are allowed. */
nunl_result_t nunl_validate_memory_map(const nunl_memory_range_t *memory_map,
                                       size_t memory_map_count);

/* CRC-32/ISO-HDLC over the fixed 48-byte header with its CRC field zeroed. */
uint32_t nunl_image_header_crc32(const void *header_bytes, size_t header_length);

/*
 * Validate a little-endian NUN image header and its target load range.
 * This routine does not copy, map, authenticate or execute the image.
 */
nunl_result_t nunl_validate_image(const void *image,
                                  size_t available_bytes,
                                  uint64_t load_address,
                                  const nunl_memory_range_t *memory_map,
                                  size_t memory_map_count,
                                  nunl_architecture_t target_architecture,
                                  uint64_t *entry_address_out);

const char *nunl_result_name(nunl_result_t result);

#ifdef __cplusplus
}
#endif

#endif
