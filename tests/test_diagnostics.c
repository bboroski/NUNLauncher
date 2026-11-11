#include "nunlauncher/diagnostics.h"
#include <assert.h>
#include <string.h>
int main(void)
{
    nunl_guidance_t g;
    assert(nunl_guidance_for_result(NUNL_OK, &g) == NUNL_OK);
    assert(g.blocking == 0u);
    assert(strcmp(g.next_action_key, "continue_only_with_separately_verified_boot_chain") == 0);
    assert(nunl_guidance_for_result(NUNL_EARCH, &g) == NUNL_OK);
    assert(g.blocking == 1u);
    assert(strcmp(g.next_action_key, "match_image_and_declared_target_architecture") == 0);
    assert(nunl_guidance_for_result(NUNL_EOVERLAP, &g) == NUNL_OK);
    assert(strcmp(g.summary_key, "memory_ranges_overlap") == 0);
    assert(nunl_guidance_for_result((nunl_result_t)77, &g) == NUNL_OK);
    assert(strcmp(g.summary_key, "validation_result_unknown") == 0 && g.blocking == 1u);
    assert(nunl_guidance_for_result(NUNL_OK, 0) == NUNL_EINVAL);
    return 0;
}
