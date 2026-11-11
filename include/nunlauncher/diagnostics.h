#ifndef NUNLAUNCHER_DIAGNOSTICS_H
#define NUNLAUNCHER_DIAGNOSTICS_H
#include "nunlauncher/boot_contract.h"
#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
    const char *summary_key;
    const char *next_action_key;
    uint8_t blocking;
} nunl_guidance_t;
/* Map validator outcomes to explicit operator guidance; performs no image I/O. */
nunl_result_t nunl_guidance_for_result(nunl_result_t result,
                                       nunl_guidance_t *guidance);
#ifdef __cplusplus
}
#endif
#endif
