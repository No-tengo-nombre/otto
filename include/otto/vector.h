#pragma once

#include <stddef.h>
#include <stdint.h>

#include "cl/cl.h"
#include "cl/runtime.h"
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
  cl_mem gmem;
  size_t data_size;
  size_t len;
  size_t capacity;
  otto_device_t device;
  const otto_runtime_t *ctx;
  cl_mem_flags flags;
} otto_vector_t;

/* Vector creation */

otto_status_t otto_vector_new(const size_t data_size, otto_vector_t *out);
otto_status_t otto_vector_zero(const size_t len, const size_t data_size,
                               otto_vector_t *out);
otto_status_t otto_vector_with_capacity(const size_t capacity,
                                        const size_t data_size,
                                        otto_vector_t *out);
otto_status_t otto_vector_from_array(const void *data, const size_t len,
                                     const size_t data_size,
                                     otto_vector_t *out);

/* Vector finalization */

otto_status_t otto_vector_cleanup(const otto_vector_t *const vec);

/* Vector attributes */

otto_status_t otto_vector_get(const otto_vector_t *vec, const size_t i,
                              void *out);
otto_status_t otto_vector_set(otto_vector_t *vec, const size_t i,
                              const void *src);
otto_status_t otto_vector_resize(otto_vector_t *vec, const size_t new_capacity);
otto_status_t otto_vector_push(otto_vector_t *vec, const void *src);
otto_status_t otto_vector_extend_array(otto_vector_t *vec, const void *src,
                                       const size_t len);

/* Vector movements */

otto_status_t otto_vector_setread(otto_vector_t *vec);
otto_status_t otto_vector_setwrite(otto_vector_t *vec);
otto_status_t otto_vector_setreadwrite(otto_vector_t *vec);

otto_status_t otto_vector_todevice_mode(otto_vector_t *vec,
                                        const otto_runtime_t *ctx,
                                        const cl_mem_flags flags);
otto_status_t otto_vector_todevice(otto_vector_t *vec,
                                   const otto_runtime_t *ctx);
otto_status_t otto_vector_todevice_read(otto_vector_t *vec,
                                        const otto_runtime_t *ctx);
otto_status_t otto_vector_todevice_write(otto_vector_t *vec,
                                         const otto_runtime_t *ctx);
otto_status_t otto_vector_tohost(otto_vector_t *vec, uint64_t total);

#ifdef __cplusplus
}
#endif
