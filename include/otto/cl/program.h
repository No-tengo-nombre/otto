#pragma once

#include <stddef.h>

#include <otto/paths.h>
#include <otto/status.h>

#include "cl.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct otto_runtime otto_runtime_t;

typedef struct otto_program {
  cl_program p;
} otto_program_t;

typedef enum otto_program_kernels {
  OTTO_KERNELS_ALL,
  OTTO_KERNELS_CORE,
} otto_program_kernels_t;

otto_status_t otto_program_from_sources(const otto_runtime_t *ctx,
                                        const char **sources,
                                        const size_t count,
                                        const char *build_options,
                                        otto_program_t *out);
otto_status_t otto_program_from_files(otto_runtime_t *ctx, const char **files,
                                      const size_t count,
                                      const char *build_options,
                                      otto_program_t *out);
otto_status_t otto_program_from_default(otto_runtime_t *ctx,
                                        const otto_program_kernels_t kernels,
                                        const char *build_options,
                                        otto_program_t *out);

otto_status_t otto_program_cleanup(const otto_program_t *prog);

otto_status_t otto_program_build(const otto_program_t *prog,
                                 const otto_runtime_t *ctx,
                                 const char *options);

#ifdef __cplusplus
}
#endif
