#include <otto/devices.h>
#include <otto/status.h>
#include <otto_utils/vendor/log.h>

otto_status_t otto_device_as_cl(const otto_device_t dev, cl_device_type *out) {
  logi_debug("Translating otto device to OpenCL device");
  switch (dev) {
  case OTTO_DEVICE_CPU:
    *out = CL_DEVICE_TYPE_CPU;
    break;
  case OTTO_DEVICE_GPU:
    *out = CL_DEVICE_TYPE_GPU;
    break;
  default: ;
    const char *msg = "Requested device not implemented";
    logi_warn(msg);
    return OTTO_STATUS_FAILURE(msg);
  }
  return OTTO_STATUS_SUCCESS;
}

otto_status_t otto_device_name(const otto_device_t dev, const char **out) {
  logi_debug("Getting name of otto device");
  switch (dev) {
  case OTTO_DEVICE_CPU:
    *out = "cpu";
    break;
  case OTTO_DEVICE_GPU:
    *out = "gpu";
    break;
  default: ;
    const char *msg = "Requested device not found";
    logi_warn(msg);
    return OTTO_STATUS_FAILURE(msg);
  }
  return OTTO_STATUS_SUCCESS;
}
