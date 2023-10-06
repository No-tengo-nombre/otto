#pragma once

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#include <otto/status.h>

#include "cl.h"

#define OTTO_VECTORARG(vec) sizeof(vec.gmem), &vec.gmem

#ifdef __cplusplus
extern "C" {
#endif

typedef struct otto_program otto_program_t;
typedef struct otto_runtime otto_runtime_t;

typedef struct otto_kernel_args {
  uint32_t work_dim;
  size_t global_size;
  size_t local_size;
} otto_kernel_args_t;

typedef struct otto_kernel {
  cl_kernel k;
  const char *name;
  size_t nargs;
} otto_kernel_t;

otto_status_t otto_kernel_new(const otto_program_t *prog, const char *name,
                              const size_t nargs, otto_runtime_t *ctx,
                              otto_kernel_t *out);

otto_status_t otto_kernel_vcall(const otto_kernel_t *ker,
                                const otto_runtime_t *ctx,
                                const otto_kernel_args_t *hparams,
                                va_list args);
otto_status_t otto_kernel_call(const otto_kernel_t *ker,
                               const otto_runtime_t *ctx,
                               const otto_kernel_args_t *hparams, ...);

#ifdef __cplusplus
}
#endif
