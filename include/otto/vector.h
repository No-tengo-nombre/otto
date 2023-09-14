#pragma once

#include <stddef.h>

#include "devices.h"
#include "status.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Vector object that can live on the CPU or GPU.
 *
 * This vector corresponds to some contiguos data, which can be stored in the
 * CPU using the heap, or the GPU using the VRAM. Note that while the data
 * always lives in the heap, the vector object itself can live in the stack.
 */
typedef struct otto_vector {
  void *data;
  size_t size;
  size_t capacity;
  otto_device_t device;
} otto_vector_t;

otto_status_t otto_vector_new(otto_vector_t *out);
otto_status_t otto_vector_with_capacity(const size_t capacity,
                                        const size_t data_size,
                                        otto_vector_t *out);
otto_status_t otto_vector_from_array(const void *data, const size_t size,
                                     const size_t data_size,
                                     otto_vector_t *out);

otto_status_t otto_vector_push(const void *target, otto_vector_t *out);
otto_status_t otto_vector_extend_array(const void *target, const size_t size,
                                       otto_vector_t *out);

#ifdef __cplusplus
}
#endif
