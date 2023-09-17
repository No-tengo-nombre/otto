#pragma once

#include <otto/cl/cl.h>
#include <otto/status.h>

#ifdef __cplusplus
extern "C" {
#endif

#define OTTO_PLATFORM_ENTRIES 1
#define OTTO_DEVICE_ENTRIES 1

typedef struct otto_runtime {
  cl_context ctx;
  cl_platform_id platforms;
  cl_uint platform_num;
  cl_device_id devices;
  cl_uint device_num;
} otto_runtime_t;

otto_status_t otto_runtime_new(const cl_context_properties *props,
                               cl_device_type type, otto_runtime_t *out);

#ifdef __cplusplus
}
#endif
