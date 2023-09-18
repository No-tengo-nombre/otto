#pragma once

#include <stddef.h>

#include <otto/status.h>

#include "cl.h"
#include "runtime.h"

typedef struct otto_program {
  cl_program p;
} otto_program_t;

otto_status_t otto_program_from_sources(const otto_runtime_t *ctx,
                                        const char **sources,
                                        const size_t count,
                                        const char *build_options,
                                        otto_program_t *out);

otto_status_t otto_program_build(const otto_program_t *prog,
                                 const otto_runtime_t *ctx,
                                 const char *options);
