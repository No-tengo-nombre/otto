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
 * @brief Buffer object that can live on the CPU or GPU.
 *
 * This buffer corresponds to some contiguos data, which can be stored in the
 * CPU using the heap, or the GPU using the VRAM. Note that while the data
 * always lives in the heap, the buffer object itself can live in the stack.
 */
typedef struct otto_buffer {
  void *data;
  cl_mem gmem;
  size_t data_size;
  size_t len;
  size_t capacity;
  otto_device_t device;
  const otto_runtime_t *ctx;
  cl_mem_flags flags;
} otto_buffer_t;

/* Buffer creation */

otto_status_t otto_buffer_new(const size_t data_size, otto_buffer_t *out);
otto_status_t otto_buffer_zero(const size_t len, const size_t data_size,
                               otto_buffer_t *out);
otto_status_t otto_buffer_with_capacity(const size_t capacity,
                                        const size_t data_size,
                                        otto_buffer_t *out);
otto_status_t otto_buffer_from_array(const void *data, const size_t len,
                                     const size_t data_size,
                                     otto_buffer_t *out);

/* Buffer finalization */

otto_status_t otto_buffer_cleanup(const otto_buffer_t *const vec);

/* Buffer attributes */

otto_status_t otto_buffer_get(const otto_buffer_t *vec, const size_t i,
                              void *out);
otto_status_t otto_buffer_set(otto_buffer_t *vec, const size_t i,
                              const void *src);
otto_status_t otto_buffer_resize(otto_buffer_t *vec, const size_t new_capacity);
otto_status_t otto_buffer_push(otto_buffer_t *vec, const void *src);
otto_status_t otto_buffer_extend_array(otto_buffer_t *vec, const void *src,
                                       const size_t len);

/* Buffer movements */

otto_status_t otto_buffer_setread(otto_buffer_t *vec);
otto_status_t otto_buffer_setwrite(otto_buffer_t *vec);
otto_status_t otto_buffer_setreadwrite(otto_buffer_t *vec);

otto_status_t otto_buffer_todevice_mode(otto_buffer_t *vec,
                                        const otto_runtime_t *ctx,
                                        const cl_mem_flags flags);
otto_status_t otto_buffer_todevice(otto_buffer_t *vec,
                                   const otto_runtime_t *ctx);
otto_status_t otto_buffer_todevice_read(otto_buffer_t *vec,
                                        const otto_runtime_t *ctx);
otto_status_t otto_buffer_todevice_write(otto_buffer_t *vec,
                                         const otto_runtime_t *ctx);
otto_status_t otto_buffer_tohost(otto_buffer_t *vec, uint64_t total);

#ifdef __cplusplus
}
#endif
