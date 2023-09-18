#include <otto/cl/cl.h>
#include <otto/cl/runtime.h>
#include <otto/status.h>
#include <otto/vector.h>
#include <otto_utils/vendor/log.h>

otto_status_t otto_vector_register(otto_vector_t *vec,
                                   const otto_runtime_t *ctx,
                                   const cl_mem_flags flags) {
  // TODO: Figure out why it is not producing the intended results
  logi_info("Registering vector in a CL context");
  if (vec == NULL) {
    logi_error("Can not register NULL vector");
    return OTTO_STATUS_FAILURE;
  }

  cl_int status = CL_SUCCESS;
  cl_mem gmem = clCreateBuffer(ctx->ctx, flags, vec->capacity * vec->data_size,
                               NULL, &status);
  if (gmem == NULL || status != CL_SUCCESS) {
    logi_error("Failed creating buffer (%d)", status);
    return OTTO_STATUS_FAILURE;
  }

  if (flags == CL_MEM_READ_ONLY) {
    logi_debug("Writing to the buffer");
    if (clEnqueueWriteBuffer(ctx->cq, gmem, CL_TRUE, 0,
                             vec->len * vec->data_size, vec->data, 0, NULL,
                             NULL) != CL_SUCCESS) {
      logi_error("Failed enqueuing buffer");
      return OTTO_STATUS_FAILURE;
    }
  }

  vec->gmem = gmem;
  vec->device = ctx->dev;
  return OTTO_STATUS_SUCCESS;
}
