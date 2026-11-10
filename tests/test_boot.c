#include "nunlauncher/boot.h"

#include <assert.h>
#include <stdio.h>

int main(void)
{
    const nunlauncher_memory_region_t regions[] = {
        {0x100000U, 0x20000U, NUNLAUNCHER_MEMORY_LOADED_KERNEL, 0U},
        {0x400000U, 0x10000U, NUNLAUNCHER_MEMORY_AVAILABLE, 0U}
    };
    nunlauncher_boot_info_t candidate = {
        NUNLAUNCHER_BOOT_INFO_MAGIC,
        NUNLAUNCHER_BOOT_INFO_VERSION,
        (uint32_t)sizeof(nunlauncher_boot_info_t),
        NUNLAUNCHER_ARCH_X86_64,
        0x100000U,
        0x20000U,
        0x100100U,
        regions,
        2U,
        0U
    };
    nunlauncher_image_descriptor_t image = {
        NUNLAUNCHER_IMAGE_MAGIC,
        NUNLAUNCHER_IMAGE_VERSION,
        (uint32_t)sizeof(nunlauncher_image_descriptor_t),
        NUNLAUNCHER_ARCH_X86_64,
        0x20000U,
        0x100000U,
        0x100U,
        0U,
        0U
    };
    nunlauncher_boot_info_t output;

    assert(nunlauncher_image_validate(&image, 0x20000U,
                                      NUNLAUNCHER_ARCH_X86_64) == NUNLAUNCHER_OK);
    assert(nunlauncher_handoff_prepare(&image, 0x20000U, &candidate,
                                       &output) == NUNLAUNCHER_OK);
    assert(output.entry_address == candidate.entry_address);

    assert(nunlauncher_image_validate(&image, 8U,
                                      NUNLAUNCHER_ARCH_X86_64) == NUNLAUNCHER_ERANGE);
    assert(nunlauncher_image_validate(&image, 0x20000U,
                                      NUNLAUNCHER_ARCH_AARCH64) == NUNLAUNCHER_EARCH);

    image.load_address = 0x200000U;
    assert(nunlauncher_handoff_prepare(&image, 0x20000U, &candidate,
                                       &output) == NUNLAUNCHER_EMISMATCH);

    puts("NUNLauncher boot contract tests: PASS");
    return 0;
}
