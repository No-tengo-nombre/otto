#include "otto_utils/macros.h"
#include "otto_utils/vendor/log.h"

#include <otto/cl/cl.h>
#include <otto/cl/runtime.h>
#include <otto/devices.h>
#include <otto/status.h>

otto_status_t otto_runtime_new(const cl_context_properties *ctx_props,
                               const cl_queue_properties *q_props,
                               otto_device_t type, otto_runtime_t *out) {
  if (out == NULL) {
    return OTTO_STATUS_FAILURE;
  }

  cl_device_type dev = CL_DEVICE_TYPE_DEFAULT;

  logi_info("Determining dev");
  switch (type) {
  case OTTO_DEVICE_CPU:
    logi_info("Using CPU");
    dev = CL_DEVICE_TYPE_CPU;
    break;
  case OTTO_DEVICE_GPU:
    logi_info("Using GPU");
    dev = CL_DEVICE_TYPE_GPU;
    break;
  default:
    logi_error("Requested device not implemented");
    return OTTO_STATUS_FAILURE;
  }

  cl_int status = CL_SUCCESS;

  cl_platform_id platforms = NULL;
  cl_uint platform_num;
  cl_device_id devices = NULL;
  cl_uint device_num;
  logi_info("Getting platforms");
  OTTO_CL_CALL_I(
      clGetPlatformIDs(OTTO_PLATFORM_ENTRIES, &platforms, &platform_num),
      "Failed getting platforms");
  logi_debug("Found %d platforms", platform_num);
  logi_info("Getting devices");
  OTTO_CL_CALL_I(clGetDeviceIDs(platforms, dev, OTTO_DEVICE_ENTRIES, &devices,
                                &device_num),
                 "Failed getting devices");
  logi_debug("Found %d devices", device_num);

  logi_info("Creating context");
  cl_context ctx =
      clCreateContext(ctx_props, device_num, &devices, NULL, NULL, &status);
  if (ctx == NULL || status != CL_SUCCESS) {
    logi_error("Failed creating context");
    return OTTO_STATUS_FAILURE;
  }

  logi_info("Creating command queue");
  cl_command_queue cq =
      clCreateCommandQueueWithProperties(ctx, devices, q_props, &status);
  if (cq == NULL || status != CL_SUCCESS) {
    logi_error("Failed creating command queue");
    return OTTO_STATUS_FAILURE;
  }

  logi_info("Assembling struct");
  otto_runtime_t result = {
      .ctx = ctx,
      .cq = cq,
      .platforms = platforms,
      .platform_num = platform_num,
      .devices = devices,
      .device_num = device_num,
      .dev = type,
  };
  *out = result;
  return OTTO_STATUS_SUCCESS;
}
