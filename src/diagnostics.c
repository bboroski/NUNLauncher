#include "nunlauncher/diagnostics.h"

nunl_result_t nunl_guidance_for_result(nunl_result_t result,
                                       nunl_guidance_t *guidance)
{
    if (guidance == NULL) return NUNL_EINVAL;
    guidance->summary_key = "validation_result_unknown";
    guidance->next_action_key = "inspect_validator_result_and_image_contract";
    guidance->blocking = 1u;
    switch (result) {
    case NUNL_OK:
        guidance->summary_key = "image_metadata_valid_for_declared_target";
        guidance->next_action_key = "continue_only_with_separately_verified_boot_chain";
        guidance->blocking = 0u;
        break;
    case NUNL_EINVAL:
        guidance->summary_key = "invalid_validation_arguments";
        guidance->next_action_key = "provide_all_required_validation_inputs";
        break;
    case NUNL_EFORMAT:
        guidance->summary_key = "image_header_format_invalid";
        guidance->next_action_key = "check_magic_flags_reserved_fields_and_header_layout";
        break;
    case NUNL_EVERSION:
        guidance->summary_key = "image_header_version_unsupported";
        guidance->next_action_key = "use_a_supported_versioned_image_header";
        break;
    case NUNL_EARCH:
        guidance->summary_key = "image_architecture_mismatch";
        guidance->next_action_key = "match_image_and_declared_target_architecture";
        break;
    case NUNL_ESIZE:
        guidance->summary_key = "image_size_invalid";
        guidance->next_action_key = "check_available_bytes_header_size_and_image_bounds";
        break;
    case NUNL_EENTRY:
        guidance->summary_key = "image_entry_offset_invalid";
        guidance->next_action_key = "place_entry_inside_image_after_header";
        break;
    case NUNL_EMEMORY:
        guidance->summary_key = "load_range_not_usable_ram";
        guidance->next_action_key = "review_aligned_load_address_and_memory_map";
        break;
    case NUNL_ECHECKSUM:
        guidance->summary_key = "header_checksum_mismatch";
        guidance->next_action_key = "regenerate_header_checksum_after_finalizing_metadata";
        break;
    case NUNL_EOVERLAP:
        guidance->summary_key = "memory_ranges_overlap";
        guidance->next_action_key = "resolve_overlapping_memory_map_ranges";
        break;
    default:
        break;
    }
    return NUNL_OK;
}
