#pragma once

#include <otto/status.h>

#include "cl.h"
#include "program.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct otto_kernel {
  cl_kernel k;
  const char *name;
} otto_kernel_t;

otto_status_t otto_kernel_new(const otto_program_t *prog, const char *name,
                              otto_kernel_t *out);

#ifdef __cplusplus
}
#endif
