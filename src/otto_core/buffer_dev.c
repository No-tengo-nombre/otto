#include <otto/buffer.h>
#include <otto/cl/cl.h>
#include <otto/cl/runtime.h>
#include <otto/devices.h>
#include <otto/status.h>
#include <otto_utils/macros.h>
#include <otto_utils/vendor/log.h>
#include <stdint.h>

otto_status_t otto_buffer_setread(otto_buffer_t *buf) {
  buf->flags = CL_MEM_READ_ONLY;
  return OTTO_STATUS_SUCCESS;
}

otto_status_t otto_buffer_setwrite(otto_buffer_t *buf) {
  buf->flags = CL_MEM_WRITE_ONLY;
  return OTTO_STATUS_SUCCESS;
}

otto_status_t otto_buffer_setreadwrite(otto_buffer_t *buf) {
  buf->flags = CL_MEM_READ_WRITE;
  return OTTO_STATUS_SUCCESS;
}

otto_status_t otto_buffer_todevice_mode(otto_buffer_t *buf, const otto_runtime_t *ctx, const cl_mem_flags flags) {
  const char *dev_name;
  OTTO_CALL_I(otto_device_name(ctx->dev, &dev_name), "Requested device not implemented");

  logi_info("Sending buffer to device '%s'", dev_name);
  if (buf == NULL) {
    logi_error("Can not register NULL buffer");
    return OTTO_STATUS_FAILURE("Can not register NULL buffer");
  }

  cl_int status = CL_SUCCESS;
  cl_mem gmem   = clCreateBuffer(ctx->ctx, flags, buf->capacity * buf->data_size, NULL, &status);
  if (gmem == NULL || status != CL_SUCCESS) {
    logi_error("Failed creating buffer (%d)", status);
    return OTTO_STATUS_FAILURE("Failed creating buffer");
  }

  if (flags == CL_MEM_READ_ONLY) {
    logi_debug("Writing %d elements (%d bytes) to device", buf->len, buf->len * buf->data_size);
    OTTO_CL_CALL_I(clEnqueueWriteBuffer(ctx->cq, gmem, CL_TRUE, 0, buf->len * buf->data_size, buf->data, 0, NULL, NULL), "Failed enqueing buffer");
  }

  buf->gmem   = gmem;
  buf->device = ctx->dev;
  buf->ctx    = ctx;
  return OTTO_STATUS_SUCCESS;
}

otto_status_t otto_buffer_todevice(otto_buffer_t *buf, const otto_runtime_t *ctx) { return otto_buffer_todevice_mode(buf, ctx, buf->flags); }

otto_status_t otto_buffer_todevice_read(otto_buffer_t *buf, const otto_runtime_t *ctx) {
  return otto_buffer_todevice_mode(buf, ctx, CL_MEM_READ_ONLY);
}

otto_status_t otto_buffer_todevice_write(otto_buffer_t *buf, const otto_runtime_t *ctx) {
  return otto_buffer_todevice_mode(buf, ctx, CL_MEM_READ_ONLY);
}

otto_status_t otto_buffer_tohost(otto_buffer_t *buf, uint64_t total) {
  if (total > buf->capacity) {
    logi_warn("Trying to fetch more data than capacity, some data will be lost");
    total = buf->capacity;
  } else if (total == 0) {
    total = buf->capacity;
  }

  logi_debug("Fetching %d elements (%d bytes) to host", total, total * buf->data_size);
  OTTO_CL_CALL_I(
      clEnqueueReadBuffer(buf->ctx->cq, buf->gmem, CL_TRUE, 0, total * buf->data_size, buf->data, 0, NULL, NULL), "Failed sending to host"
  );
  buf->len = total;
  return OTTO_STATUS_SUCCESS;
}
