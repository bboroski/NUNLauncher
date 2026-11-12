#include "nunlauncher/preflight_ledger.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
int main(void)
{
    nunl_preflight_ledger_t ledger;
    nunl_preflight_event_t rows[NUNL_PREFLIGHT_CAPACITY];
    size_t i, count;
    nunl_preflight_ledger_init(&ledger);
    assert(nunl_preflight_ledger_append(&ledger, 10u, 1u, 7u, 1u, 900u, NUNL_PREFLIGHT_FLAG_OBSERVED, 0u) == 1);
    assert(nunl_preflight_ledger_append(&ledger, 11u, 2u, 7u, 2u, 800u,
        NUNL_PREFLIGHT_FLAG_OBSERVED | NUNL_PREFLIGHT_FLAG_CORROBORATED, 1u) == 1);
    assert(nunl_preflight_ledger_append(&ledger, 9u, 3u, 7u, 1u, 500u, NUNL_PREFLIGHT_FLAG_OBSERVED, 0u) == 0);
    assert(nunl_preflight_ledger_append(&ledger, 12u, 3u, 7u, 1u, 1001u, NUNL_PREFLIGHT_FLAG_OBSERVED, 0u) == 0);
    assert(nunl_preflight_ledger_append(&ledger, 12u, 3u, 7u, 1u, 500u, NUNL_PREFLIGHT_FLAG_CORROBORATED, 0u) == 0);
    assert(nunl_preflight_ledger_append(&ledger, 12u, 3u, 7u, 1u, 500u, NUNL_PREFLIGHT_FLAG_OBSERVED, ledger.next_sequence) == 0);
    assert(nunl_preflight_ledger_snapshot(&ledger, NULL, 0u) == 2u);
    count = nunl_preflight_ledger_snapshot(&ledger, rows, NUNL_PREFLIGHT_CAPACITY);
    assert(count == 2u && rows[0].sequence == 1u && rows[1].sequence == 2u);
    for (i = 0u; i < NUNL_PREFLIGHT_CAPACITY + 3u; ++i)
        assert(nunl_preflight_ledger_append(&ledger, 20u + (uint64_t)i, 10u + (uint32_t)i,
            7u, 0u, 1000u, NUNL_PREFLIGHT_FLAG_OBSERVED, 0u) == 1);
    assert(ledger.count == NUNL_PREFLIGHT_CAPACITY && ledger.dropped == 5u);
    count = nunl_preflight_ledger_snapshot(&ledger, rows, NUNL_PREFLIGHT_CAPACITY);
    assert(count == NUNL_PREFLIGHT_CAPACITY && rows[0].sequence == 8u);
    puts("bounded event history tests passed");
    return 0;
}
