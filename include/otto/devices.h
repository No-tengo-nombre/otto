#pragma once

#include "cl/cl.h"
#include "status.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Devices in which an object can live.
 */
typedef enum otto_device {
  OTTO_DEVICE_CPU,
  OTTO_DEVICE_GPU,
} otto_device_t;

otto_status_t otto_device_as_cl(const otto_device_t dev, cl_device_type *out);
otto_status_t otto_device_name(const otto_device_t dev, const char **out);

#ifdef __cplusplus
}
#endif
