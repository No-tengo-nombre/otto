#pragma once

#include <stddef.h>

#include "devices.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Tensor object that can live on the CPU or GPU.
 *
 * This type represents a general tensor, which is an n-dimensional grouping of
 * data. Examples of tensors are vectors (rank 1) and matrices (rank 2), but
 * higher ranks can be considered.
 *
 * These tensors can live either in the CPU, where the contents are heap
 * allocated, or in the GPU, where they live in the VRAM.
 */
typedef struct otto_tensor {
  void *data;
  size_t size;
  size_t capacity;
  otto_device_t device;
} otto_tensor_t;

#ifdef __cplusplus
}
#endif
