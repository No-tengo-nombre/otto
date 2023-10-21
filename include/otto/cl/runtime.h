#pragma once

#include <stdarg.h>

#include <uthash.h>

#include <otto/cl/cl.h>
#include <otto/devices.h>
#include <otto/status.h>

#define OTTO_PLATFORM_ENTRIES 1
#define OTTO_DEVICE_ENTRIES 1

#ifdef __cplusplus
extern "C" {
#endif

typedef struct otto_kernel_args otto_kernel_args_t;
typedef struct otto_kernel otto_kernel_t;
typedef struct otto_vector otto_vector_t;

typedef struct otto_kernelht {
  const char *name;
  otto_kernel_t *kernel;
  UT_hash_handle hh;
} otto_kernelht_t;

typedef struct otto_kernelll {
  otto_kernelht_t *item;
  struct otto_kernelll *next;
} otto_kernelll_t;

otto_status_t otto_kernelll_push(otto_kernelll_t *head, otto_kernelht_t *val);
otto_status_t otto_kernelll_cleanup(otto_kernelll_t *head);

/**
 * @brief Context for a program that uses OpenCL
 *
 * This context should be initialized once in every program. It
 * handles OpenCL devices, platforms and holds all relevant attributes
 * that OpenCL might use, such as the OpenCL context and command queue.
 *
 * The attribute `_kernels_ht` is a hash table with all the kernels that
 * are registered in the given context, to allow calling kernels from
 * their name.
 *
 * The attribute `_kernels_ll` is a linked list where each element is a pointer
 * to an `otto_kernel_t` element. This is done to facilitate the releasing of
 * all the memory.
 */
typedef struct otto_runtime {
  cl_context ctx;
  cl_command_queue cq;
  cl_platform_id platforms;
  cl_uint platform_num;
  cl_device_id devices;
  cl_uint device_num;
  otto_device_t dev;
  otto_kernelht_t *_kernels_ht;
  otto_kernelll_t *_kernels_ll;
  otto_kernel_args_t *kernel_hparams;
  const char **_sources; // Keeps track of them only to clean them up
  size_t _sources_count; // Keeps track of them only to clean them up
} otto_runtime_t;

otto_status_t otto_runtime_new(const cl_context_properties *ctx_props,
                               const cl_queue_properties *q_props,
                               const otto_device_t type,
                               otto_kernelht_t *kernel_ht, otto_runtime_t *out);

otto_status_t otto_runtime_cleanup(const otto_runtime_t *ctx);

otto_status_t otto_runtime_add_kernel(otto_runtime_t *ctx, const char *name,
                                      otto_kernel_t *kernel);
otto_status_t otto_runtime_get_kernel(const otto_runtime_t *ctx,
                                      const char *name, otto_kernel_t *out);
otto_status_t otto_runtime_vcall_kernel(const otto_runtime_t *ctx,
                                        const char *name,
                                        const otto_kernel_args_t *hparams,
                                        va_list args);
otto_status_t otto_runtime_call_kernel(const otto_runtime_t *ctx,
                                       const char *name,
                                       const otto_kernel_args_t *hparams, ...);
otto_status_t otto_runtime_call_kernel_binop(const otto_runtime_t *ctx,
                                             const char *name,
                                             const otto_kernel_args_t *hparams,
                                             const otto_vector_t *a,
                                             const otto_vector_t *b,
                                             otto_vector_t *out);

#ifdef __cplusplus
}
#endif
