#pragma once

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

#ifdef __cplusplus
}
#endif
