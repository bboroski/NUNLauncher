#include "nunlauncher/boot_contract.h"

#include <assert.h>
#include <stdint.h>
#include <string.h>

static void put_le32(uint8_t *bytes, uint32_t value)
{
    bytes[0] = (uint8_t)value;
    bytes[1] = (uint8_t)(value >> 8);
    bytes[2] = (uint8_t)(value >> 16);
    bytes[3] = (uint8_t)(value >> 24);
}

static void put_le64(uint8_t *bytes, uint64_t value)
{
    put_le32(bytes, (uint32_t)value);
    put_le32(bytes + 4u, (uint32_t)(value >> 32));
}

static void make_image(uint8_t image[64], nunl_architecture_t architecture)
{
    uint32_t crc;
    memset(image, 0, 64u);
    memcpy(image, NUNL_IMAGE_MAGIC, NUNL_IMAGE_MAGIC_SIZE);
    put_le32(image + 8u, 1u);
    put_le32(image + 12u, NUNL_IMAGE_HEADER_SIZE);
    put_le64(image + 16u, 64u);
    put_le64(image + 24u, NUNL_IMAGE_HEADER_SIZE);
    put_le32(image + 32u, (uint32_t)architecture);
    crc = nunl_image_header_crc32(image, 64u);
    put_le32(image + 40u, crc);
}

int main(void)
{
    uint8_t image[64];
    uint64_t entry = 0u;
    nunl_memory_range_t ram = {
        UINT64_C(0x00100000), UINT64_C(0x00002000),
        NUNL_MEMORY_USABLE_RAM, 0u
    };
    nunl_result_t result;

    make_image(image, NUNL_ARCH_X86_64);
    result = nunl_validate_image(image, sizeof(image), UINT64_C(0x00100000),
                                 &ram, 1u, NUNL_ARCH_X86_64, &entry);
    assert(result == NUNL_OK);
    assert(entry == UINT64_C(0x00100000) + NUNL_IMAGE_HEADER_SIZE);

    result = nunl_validate_image(image, sizeof(image), UINT64_C(0x00100000),
                                 &ram, 1u, NUNL_ARCH_AARCH64, &entry);
    assert(result == NUNL_EARCH);

    image[60] ^= 0x01u;
    result = nunl_validate_image(image, sizeof(image), UINT64_C(0x00100000),
                                 &ram, 1u, NUNL_ARCH_X86_64, &entry);
    assert(result == NUNL_OK); /* Payload bytes are outside the header CRC. */

    image[12] = 0u;
    result = nunl_validate_image(image, sizeof(image), UINT64_C(0x00100000),
                                 &ram, 1u, NUNL_ARCH_X86_64, &entry);
    assert(result == NUNL_EVERSION);

    make_image(image, NUNL_ARCH_X86_64);
    image[40] ^= 0x80u;
    result = nunl_validate_image(image, sizeof(image), UINT64_C(0x00100000),
                                 &ram, 1u, NUNL_ARCH_X86_64, &entry);
    assert(result == NUNL_ECHECKSUM);

    make_image(image, NUNL_ARCH_X86_64);
    put_le64(image + 24u, 64u);
    put_le32(image + 40u, nunl_image_header_crc32(image, sizeof(image)));
    result = nunl_validate_image(image, sizeof(image), UINT64_C(0x00100000),
                                 &ram, 1u, NUNL_ARCH_X86_64, &entry);
    assert(result == NUNL_EENTRY);

    make_image(image, NUNL_ARCH_X86_64);
    result = nunl_validate_image(image, sizeof(image), UINT64_C(0x00100001),
                                 &ram, 1u, NUNL_ARCH_X86_64, &entry);
    assert(result == NUNL_EMEMORY);

    ram.size = 32u;
    result = nunl_validate_image(image, sizeof(image), UINT64_C(0x00100000),
                                 &ram, 1u, NUNL_ARCH_X86_64, &entry);
    assert(result == NUNL_EMEMORY);

    {
        const nunl_memory_range_t adjacent[] = {
            {UINT64_C(0x00100000), UINT64_C(0x00002000), NUNL_MEMORY_USABLE_RAM, 0u},
            {UINT64_C(0x00102000), UINT64_C(0x00001000), NUNL_MEMORY_RESERVED, 0u}
        };
        const nunl_memory_range_t overlapping[] = {
            {UINT64_C(0x00100000), UINT64_C(0x00002000), NUNL_MEMORY_USABLE_RAM, 0u},
            {UINT64_C(0x00101000), UINT64_C(0x00001000), NUNL_MEMORY_RESERVED, 0u}
        };
        const nunl_memory_range_t overflowing[] = {
            {UINT64_MAX - UINT64_C(3), UINT64_C(8), NUNL_MEMORY_USABLE_RAM, 0u}
        };
        assert(nunl_validate_memory_map(adjacent, 2u) == NUNL_OK);
        assert(nunl_validate_memory_map(overlapping, 2u) == NUNL_EOVERLAP);
        assert(nunl_validate_memory_map(overflowing, 1u) == NUNL_EMEMORY);
        assert(nunl_validate_memory_map(NULL, 1u) == NUNL_EINVAL);

        make_image(image, NUNL_ARCH_X86_64);
        assert(nunl_validate_image(image, sizeof(image), UINT64_C(0x00100000),
                                   overlapping, 2u, NUNL_ARCH_X86_64, &entry)
               == NUNL_EOVERLAP);
    }

    assert(strcmp(nunl_result_name(NUNL_OK), "ok") == 0);
    assert(nunl_image_header_crc32(NULL, NUNL_IMAGE_HEADER_SIZE) == 0u);
    assert(strcmp(nunl_result_name(NUNL_EOVERLAP), "overlapping_memory_ranges") == 0);
    return 0;
}
