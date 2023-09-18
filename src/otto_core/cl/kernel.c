#include <otto/cl/cl.h>
#include <otto/cl/kernel.h>
#include <otto/cl/program.h>
#include <otto/status.h>
#include <otto_utils/vendor/log.h>

otto_status_t otto_kernel_new(const otto_program_t *prog, const char *name,
                              otto_kernel_t *out) {
  cl_int status = CL_SUCCESS;
  cl_kernel k = clCreateKernel(prog->p, name, &status);
  if (k == NULL || status != CL_SUCCESS) {
    logi_error("Failed creating kernel '%s' (%d)", name, status);
    return OTTO_STATUS_FAILURE;
  }

  // TODO: Implement registering the kernel in a context

  otto_kernel_t kernel = {
      .k = k,
      .name = name,
  };
  *out = kernel;
  return OTTO_STATUS_FAILURE;
}
