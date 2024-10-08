#include <stdarg.h>
#include <stddef.h>

#include <otto/cl/cl.h>
#include <otto/cl/kernel.h>
#include <otto/cl/program.h>
#include <otto/cl/runtime.h>
#include <otto/status.h>
#include <otto/buffer.h>
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
    return OTTO_STATUS_FAILURE("Failed creating kernel");
  }

  logi_info("Allocating memory for the kernel");
  otto_kernel_t *kernel_ptr = malloc(sizeof(otto_kernel_t));
  if (kernel_ptr == NULL) {
    logi_error("Could not allocate memory");
    return OTTO_STATUS_FAILURE("Could not allocate memory");
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
              "Failed adding buffer to hash table");

  if (out != NULL) {
    logi_info("Copying output");
    *out = kernel;
  }
  return OTTO_STATUS_SUCCESS;
}

otto_status_t otto_kernel_vcall(const otto_kernel_t *ker,
                                const otto_runtime_t *ctx,
                                const otto_kernel_args_t *hparams,
                                va_list args) {
  if (ker == NULL) {
    logi_error("Can not call NULL pointer as kernel");
    return OTTO_STATUS_FAILURE("Can not call NULL pointer as kernel");
  }

  logi_info("Calling kernel '%s'", ker->name);

  for (uint32_t i = 0; i < ker->nargs; i++) {
    size_t size = va_arg(args, size_t);
    void *arg = va_arg(args, void *);
    OTTO_CL_CALL_I(clSetKernelArg(ker->k, i, size, arg),
                   "Failed passing %d-th arg to the kernel", i);
  }

  enum otto_status s = OTTO_SUCCESS;
  // Try to call with passed hparams first
  if (hparams != NULL) {
    logi_debug("Calling with given hparams");
    s = clEnqueueNDRangeKernel(ctx->cq, ker->k, hparams->work_dim, NULL,
                               &hparams->global_size, &hparams->local_size, 0,
                               NULL, NULL);
    if (s != OTTO_SUCCESS) {
      logi_warn("Calling with given hparams failed");
    }
  }

  // Call with derived hparams if none were passed or calling with passed failed
  if ((hparams == NULL || s == OTTO_FAILURE) && ctx->kernel_hparams != NULL) {
    logi_debug("Defaulting to derived hparams");
    s = clEnqueueNDRangeKernel(ctx->cq, ker->k, ctx->kernel_hparams->work_dim,
                               NULL, &ctx->kernel_hparams->global_size,
                               &ctx->kernel_hparams->local_size, 0, NULL, NULL);
    if (s != OTTO_SUCCESS) {
      logi_error("Calling with derived hparams failed");
      return OTTO_STATUS_FAILURE("Calling kernel witwh derived hparams failed");
    }
  }

  // Fail if no hparams were found
  if (hparams == NULL && ctx->kernel_hparams == NULL) {
    logi_error("No hyperparameters were found. Either pass them as an argument "
               "to use them locally, or keep them in the `kernel_hparams` "
               "attribute in the runtime to use them globally");
    return OTTO_STATUS_FAILURE("No hparams found");
  }

  return OTTO_STATUS_SUCCESS;
}

otto_status_t otto_kernel_call(const otto_kernel_t *ker,
                               const otto_runtime_t *ctx,
                               const otto_kernel_args_t *hparams, ...) {
  va_list args;
  va_start(args, hparams);
  otto_status_t s = otto_kernel_vcall(ker, ctx, hparams, args);
  va_end(args);
  return s;
}

otto_status_t otto_kernel_call_binop(const otto_kernel_t *ker,
                                     const otto_runtime_t *ctx,
                                     const otto_kernel_args_t *hparams,
                                     const otto_buffer_t *a,
                                     const otto_buffer_t *b,
                                     otto_buffer_t *out) {
  return otto_kernel_call(ker, ctx, hparams, OTTO_PVECTORARG(a),
                          OTTO_PVECTORARG(b), OTTO_PVECTORARG(out));
}
