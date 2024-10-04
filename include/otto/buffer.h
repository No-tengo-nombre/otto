#pragma once

#include "cl/cl.h"
#include "cl/runtime.h"
#include "devices.h"
#include "status.h"

#include <stddef.h>
#include <stdint.h>

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
  void                 *data;
  cl_mem                gmem;
  size_t                data_size;
  size_t                len;
  size_t                capacity;
  otto_device_t         device;
  const otto_runtime_t *ctx;
  cl_mem_flags          flags;
} otto_buffer_t;

/* Buffer creation */

otto_status_t otto_buffer_new(const size_t data_size, otto_buffer_t *out);
otto_status_t otto_buffer_zero(const size_t len, const size_t data_size, otto_buffer_t *out);
otto_status_t otto_buffer_with_capacity(const size_t capacity, const size_t data_size, otto_buffer_t *out);
otto_status_t otto_buffer_from_array(const void *data, const size_t len, const size_t data_size, otto_buffer_t *out);

otto_status_t otto_buffer_cleanup(const otto_buffer_t *const buf);

otto_status_t otto_buffer_get(const otto_buffer_t *buf, const size_t i, void *out);
otto_status_t otto_buffer_set(otto_buffer_t *buf, const size_t i, const void *src);
otto_status_t otto_buffer_push(otto_buffer_t *buf, const void *src);
otto_status_t otto_buffer_resize(otto_buffer_t *buf, const size_t new_capacity);
otto_status_t otto_buffer_extend_array(otto_buffer_t *buf, const void *src, const size_t len);

int8_t   otto_buffer_get_i8(const otto_buffer_t *buf, const size_t i);
int16_t  otto_buffer_get_i16(const otto_buffer_t *buf, const size_t i);
int32_t  otto_buffer_get_i32(const otto_buffer_t *buf, const size_t i);
int64_t  otto_buffer_get_i64(const otto_buffer_t *buf, const size_t i);
uint8_t  otto_buffer_get_u8(const otto_buffer_t *buf, const size_t i);
uint16_t otto_buffer_get_u16(const otto_buffer_t *buf, const size_t i);
uint32_t otto_buffer_get_u32(const otto_buffer_t *buf, const size_t i);
uint64_t otto_buffer_get_u64(const otto_buffer_t *buf, const size_t i);
float    otto_buffer_get_f(const otto_buffer_t *buf, const size_t i);
double   otto_buffer_get_d(const otto_buffer_t *buf, const size_t i);
void     otto_buffer_set_i8(otto_buffer_t *buf, const size_t i, const int8_t src);
void     otto_buffer_set_i16(otto_buffer_t *buf, const size_t i, const int16_t src);
void     otto_buffer_set_i32(otto_buffer_t *buf, const size_t i, const int32_t src);
void     otto_buffer_set_i64(otto_buffer_t *buf, const size_t i, const int64_t src);
void     otto_buffer_set_u8(otto_buffer_t *buf, const size_t i, const uint8_t src);
void     otto_buffer_set_u16(otto_buffer_t *buf, const size_t i, const uint16_t src);
void     otto_buffer_set_u32(otto_buffer_t *buf, const size_t i, const uint32_t src);
void     otto_buffer_set_u64(otto_buffer_t *buf, const size_t i, const uint64_t src);
void     otto_buffer_set_f(otto_buffer_t *buf, const size_t i, const float src);
void     otto_buffer_set_d(otto_buffer_t *buf, const size_t i, const double src);
void     otto_buffer_push_i8(otto_buffer_t *buf, const int8_t src);
void     otto_buffer_push_i16(otto_buffer_t *buf, const int16_t src);
void     otto_buffer_push_i32(otto_buffer_t *buf, const int32_t src);
void     otto_buffer_push_i64(otto_buffer_t *buf, const int64_t src);
void     otto_buffer_push_u8(otto_buffer_t *buf, const uint8_t src);
void     otto_buffer_push_u16(otto_buffer_t *buf, const uint16_t src);
void     otto_buffer_push_u32(otto_buffer_t *buf, const uint32_t src);
void     otto_buffer_push_u64(otto_buffer_t *buf, const uint64_t src);
void     otto_buffer_push_f(otto_buffer_t *buf, const float src);
void     otto_buffer_push_d(otto_buffer_t *buf, const double src);

/* Buffer movements */

otto_status_t otto_buffer_setread(otto_buffer_t *buf);
otto_status_t otto_buffer_setwrite(otto_buffer_t *buf);
otto_status_t otto_buffer_setreadwrite(otto_buffer_t *buf);

otto_status_t otto_buffer_todevice_mode(otto_buffer_t *buf, const otto_runtime_t *ctx, const cl_mem_flags flags);
otto_status_t otto_buffer_todevice(otto_buffer_t *buf, const otto_runtime_t *ctx);
otto_status_t otto_buffer_todevice_read(otto_buffer_t *buf, const otto_runtime_t *ctx);
otto_status_t otto_buffer_todevice_write(otto_buffer_t *buf, const otto_runtime_t *ctx);
otto_status_t otto_buffer_tohost(otto_buffer_t *buf, uint64_t total);

#ifdef __cplusplus
}
#endif
