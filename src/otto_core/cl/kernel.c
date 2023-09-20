#include <stdarg.h>
#include <stddef.h>

#include <otto/cl/cl.h>
#include <otto/cl/kernel.h>
#include <otto/cl/program.h>
#include <otto/cl/runtime.h>
#include <otto/status.h>
#include <otto_utils/macros.h>
#include <otto_utils/vendor/log.h>

otto_status_t otto_kernel_new(const otto_program_t *prog, const char *name,
                              const size_t nargs, otto_runtime_t *ctx,
                              otto_kernel_t *out) {
  cl_int status = CL_SUCCESS;
  logi_info("Creating kernel");
  cl_kernel k = clCreateKernel(prog->p, name, &status);
  if (k == NULL || status != CL_SUCCESS) {
    logi_error("Failed creating kernel '%s' (%d)", name, status);
    return OTTO_STATUS_FAILURE;
  }

  logi_info("Allocating memory for the kernel");
  otto_kernel_t *kernel_ptr = malloc(sizeof(otto_kernel_t));
  if (kernel_ptr == NULL) {
    logi_error("Could not allocate memory");
    return OTTO_STATUS_FAILURE;
  }

  logi_info("Assembling struct");
  otto_kernel_t kernel = {
      .k = k,
      .name = name,
      .nargs = nargs,
  };
  *kernel_ptr = kernel;
  logi_info("Adding kernel to runtime");
  OTTO_CALL_I(otto_runtime_add_kernel(ctx, name, kernel_ptr),
              "Failed adding vector to hash table");

  if (out != NULL) {
    logi_info("Copying output");
    *out = kernel;
  }
  return OTTO_STATUS_SUCCESS;
}

otto_status_t otto_kernel_call(const otto_kernel_t *ker, ...) {
  if (ker == NULL) {
    logi_error("Can not call NULL pointer as kernel");
    return OTTO_STATUS_FAILURE;
  }

  logi_info("Calling kernel '%s'", ker->name);

  va_list args;
  va_start(args, ker);

  for (int i = 0; i < ker->nargs; i++) {
    void *arg = va_arg(args, void *);
    OTTO_CL_CALL_I(clSetKernelArg(ker->k, i, sizeof(*arg), arg),
                   "Failed passing %d-th arg to the kernel");
  }

  return OTTO_STATUS_SUCCESS;
}
