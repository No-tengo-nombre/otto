#include <stdint.h>

#include <otto/cl/cl.h>
#include <otto/cl/runtime.h>
#include <otto/devices.h>
#include <otto/status.h>
#include <otto/vector.h>
#include <otto_utils/macros.h>
#include <otto_utils/vendor/log.h>

otto_status_t otto_vector_todevice_mode(otto_vector_t *vec,
                                        const otto_runtime_t *ctx,
                                        const cl_mem_flags flags) {
  const char *dev_name;
  OTTO_CALL_I(otto_device_name(ctx->dev, &dev_name),
              "Requested device not implemented");

  logi_info("Sending vector to device '%s'", dev_name);
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
    logi_debug("Writing %d elements (%d bytes) to device", vec->len,
               vec->len * vec->data_size);
    OTTO_CL_CALL_I(clEnqueueWriteBuffer(ctx->cq, gmem, CL_TRUE, 0,
                                        vec->len * vec->data_size, vec->data, 0,
                                        NULL, NULL),
                   "Failed enqueing buffer");
  }

  vec->gmem = gmem;
  vec->device = ctx->dev;
  vec->ctx = ctx;
  return OTTO_STATUS_SUCCESS;
}

otto_status_t otto_vector_todevice(otto_vector_t *vec,
                                   const otto_runtime_t *ctx) {
  return otto_vector_todevice_mode(vec, ctx, CL_MEM_READ_WRITE);
}

otto_status_t otto_vector_todevice_read(otto_vector_t *vec,
                                        const otto_runtime_t *ctx) {
  return otto_vector_todevice_mode(vec, ctx, CL_MEM_READ_ONLY);
}

otto_status_t otto_vector_todevice_write(otto_vector_t *vec,
                                         const otto_runtime_t *ctx) {
  return otto_vector_todevice_mode(vec, ctx, CL_MEM_READ_ONLY);
}

otto_status_t otto_vector_tohost(otto_vector_t *vec, uint64_t total) {
  if (total > vec->capacity) {
    logi_warn(
        "Trying to fetch more data than capacity, some data will be lost");
    total = vec->capacity;
  } else if (total == 0) {
    total = vec->capacity;
  }

  logi_debug("Fetching %d elements (%d bytes) to host", total,
             total * vec->data_size);
  OTTO_CL_CALL_I(clEnqueueReadBuffer(vec->ctx->cq, vec->gmem, CL_TRUE, 0,
                                     total * vec->data_size, vec->data, 0, NULL,
                                     NULL),
                 "Failed sending to host");
  vec->len = total;
  return OTTO_STATUS_SUCCESS;
}
