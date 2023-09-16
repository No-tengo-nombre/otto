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
  size_t data_size;
  size_t len;
  size_t capacity;
  otto_device_t device;
} otto_vector_t;

otto_status_t otto_vector_new(const size_t data_size, otto_vector_t *out);
otto_status_t otto_vector_zero(const size_t len, const size_t data_size,
                               otto_vector_t *out);
otto_status_t otto_vector_with_capacity(const size_t capacity,
                                        const size_t data_size,
                                        otto_vector_t *out);
otto_status_t otto_vector_from_array(const void *data, const size_t len,
                                     const size_t data_size,
                                     otto_vector_t *out);

otto_status_t otto_vector_cleanup(const otto_vector_t *const vec);

otto_status_t otto_vector_get(const otto_vector_t *vec, const size_t i,
                              void *out);
otto_status_t otto_vector_set(otto_vector_t *vec, const size_t i,
                              const void *src);

otto_status_t otto_vector_push(const void *src, otto_vector_t *out);
otto_status_t otto_vector_extend_array(const void *src, const size_t len,
                                       otto_vector_t *out);

#ifdef __cplusplus
}
#endif
