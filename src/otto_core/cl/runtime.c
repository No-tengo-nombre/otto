#include <CL/cl.h>
#include <otto/cl/cl.h>
#include <otto/cl/runtime.h>
#include <otto/status.h>

otto_status_t otto_runtime_new(const cl_context_properties *props,
                               cl_device_type type, otto_runtime_t *out) {
  if (out == NULL) {
    return OTTO_STATUS_FAILURE;
  }

  cl_int status = CL_SUCCESS;

  cl_platform_id platforms = NULL;
  cl_uint platform_num;
  cl_device_id devices = NULL;
  cl_uint device_num;
  status |= clGetPlatformIDs(OTTO_PLATFORM_ENTRIES, &platforms, &platform_num);
  status |= clGetDeviceIDs(platforms, type, OTTO_DEVICE_ENTRIES, &devices,
                           &device_num);
  if (status != CL_SUCCESS) {
    return OTTO_STATUS_FAILURE;
  }

  cl_context ctx =
      clCreateContext(props, device_num, &devices, NULL, NULL, &status);

  if (ctx == NULL || status != CL_SUCCESS) {
    return OTTO_STATUS_FAILURE;
  }

  otto_runtime_t result = {
      .ctx = ctx,
      .platforms = platforms,
      .platform_num = platform_num,
      .devices = devices,
      .device_num = device_num,
  };
  *out = result;
  return OTTO_STATUS_SUCCESS;
}
