#define _POSIX_C_SOURCE 200809L
#include <jni.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include "nun/runtime.h"

#define NUN_ANDROID_ARENA_SIZE (64u * 1024u)

typedef struct nun_android_host {
    uint64_t completed_steps;
} nun_android_host_t;

static nun_runtime_t g_runtime;
static nun_android_host_t g_host;
static int g_initialized;
static union {
    max_align_t alignment;
    unsigned char bytes[NUN_ANDROID_ARENA_SIZE];
} g_arena;

static pbsc_status_t android_monotonic_ticks(void *context, uint64_t *ticks_out)
{
    struct timespec now;
    (void)context;
    if (ticks_out == NULL || clock_gettime(CLOCK_MONOTONIC, &now) != 0) {
        return PBSC_EINVAL;
    }
    *ticks_out = ((uint64_t)now.tv_sec * UINT64_C(1000)) +
                 ((uint64_t)now.tv_nsec / UINT64_C(1000000));
    return PBSC_OK;
}

static dlo_result_t android_host_init(void *context)
{
    nun_android_host_t *host = (nun_android_host_t *)context;
    if (host == NULL) return DLO_EINVAL;
    host->completed_steps = 0u;
    return DLO_OK;
}

static dlo_result_t android_host_step(void *context)
{
    nun_android_host_t *host = (nun_android_host_t *)context;
    if (host == NULL) return DLO_EINVAL;
    if (host->completed_steps < UINT64_MAX) ++host->completed_steps;
    return DLO_OK;
}

static void android_host_shutdown(void *context)
{
    (void)context;
}

JNIEXPORT jint JNICALL Java_com_nun_launcher_NunCore_nativeStart(JNIEnv *env, jclass type)
{
    pbsc_platform_ops_t platform;
    dlo_hooks_t hooks;
    nun_runtime_config_t config;
    nun_result_t result;
    (void)env;
    (void)type;

    if (g_initialized) return NUN_OK;

    platform.abi_version = PBSC_ABI_VERSION;
    platform.struct_size = (uint32_t)sizeof(platform);
    platform.capabilities = PBSC_PLATFORM_CAP_MONOTONIC_CLOCK;
    platform.context = NULL;
    platform.ticks_read = android_monotonic_ticks;
    platform.irq_save = NULL;
    platform.irq_restore = NULL;
    platform.device_barrier = NULL;

    hooks.context = &g_host;
    hooks.init = android_host_init;
    hooks.step = android_host_step;
    hooks.shutdown = android_host_shutdown;

    config.platform = &platform;
    config.lifecycle_hooks = &hooks;
    config.arena_storage = g_arena.bytes;
    config.arena_capacity = sizeof(g_arena.bytes);
    config.memory_limit = 48u * 1024u;
    config.queue_limit = 128u;
    config.work_limit = UINT64_C(1000000000);
    config.heartbeat_timeout_ticks = 15000u;

    result = nun_runtime_init(&g_runtime, &config);
    if (result != NUN_OK) return (jint)result;
    result = nun_runtime_start(&g_runtime);
    if (result != NUN_OK) {
        (void)nun_runtime_stop(&g_runtime);
        return (jint)result;
    }
    g_initialized = 1;
    return NUN_OK;
}

JNIEXPORT jint JNICALL Java_com_nun_launcher_NunCore_nativePoll(JNIEnv *env, jclass type)
{
    (void)env;
    (void)type;
    if (!g_initialized) return NUN_ESTATE;
    return (jint)nun_runtime_poll(&g_runtime);
}

JNIEXPORT jint JNICALL Java_com_nun_launcher_NunCore_nativeStop(JNIEnv *env, jclass type)
{
    nun_result_t result;
    (void)env;
    (void)type;
    if (!g_initialized) return NUN_OK;
    result = nun_runtime_stop(&g_runtime);
    if (result == NUN_OK) g_initialized = 0;
    return (jint)result;
}

JNIEXPORT jint JNICALL Java_com_nun_launcher_NunCore_nativeState(JNIEnv *env, jclass type)
{
    (void)env;
    (void)type;
    if (!g_initialized) return NUN_STATE_COLD;
    return (jint)nun_runtime_state(&g_runtime);
}
