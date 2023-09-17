#include <otto/cl/cl.h>
#include <otto/cl/runtime.h>
#include <otto/status.h>
#include <otto/vector.h>

otto_status_t otto_vector_register(otto_vector_t *vec, cl_mem_flags flags,
                                   const otto_runtime_t *ctx) {
  // TODO: Implement moving devices

  if (vec == NULL) {
    return OTTO_STATUS_FAILURE;
  }

  cl_int status = CL_SUCCESS;
  cl_mem gmem =
      clCreateBuffer(ctx->ctx, flags, vec->len * vec->data_size, NULL, &status);
  if (gmem == NULL || status != CL_SUCCESS) {
    return OTTO_STATUS_FAILURE;
  }

  if (clEnqueueWriteBuffer(ctx->cq, gmem, CL_TRUE, 0, vec->len * vec->data_size,
                           vec->data, 0, NULL, NULL) != CL_SUCCESS) {
    return OTTO_STATUS_FAILURE;
  }
  vec->gmem = gmem;

  return OTTO_STATUS_SUCCESS;
}
