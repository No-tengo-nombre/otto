#pragma once

#include <stddef.h>

#include "devices.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Vector object that can live on the CPU or GPU.
 *
 * This vector corresponds to some contiguos data, which can be stored in the
 * CPU using the heap, or the GPU using the VRAM.
 */
typedef struct otto_vector {
  void *data;
  size_t size;
  size_t capacity;
  otto_device_t device;
} otto_vector_t;

#ifdef __cplusplus
}
#endif
