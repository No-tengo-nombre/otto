#pragma once

#include "buffer.h"
#include "status.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Tensor object that generalizes a buffer to any rank.
 *
 * Tensors are designed to contain otto buffers while providing abstractions
 * that facilitate doing mathematical work with them. They wrap some contiguous
 * data, while containing higher level information about what said data
 * represents (e.g. nine contiguous floats can correspond to a length 9 buffer
 * or a 3x3 matrix).
 *
 * The `shape` attribute corresponds to an array of `rank` elements describing
 * the shape of the underlying data.
 */
typedef struct otto_tensor {
  otto_buffer_t buf;
  size_t        rank;
  size_t       *shape;
} otto_tensor_t;

/* Tensor creation */

// otto_status_t otto_tensor_new(otto_tensor_t *out);
otto_status_t otto_tensor_zero(const size_t rank, size_t *shape, const size_t data_size, otto_tensor_t *out);
otto_status_t otto_tensor_from_array(const void *data, const size_t rank, size_t *shape, const size_t data_size, otto_tensor_t *out);

/* Tensor finalization */
otto_status_t otto_tensor_cleanup(const otto_tensor_t *const mat);

/* Tensor attributes */

otto_status_t otto_tensor_calculate_index(const otto_tensor_t *mat, const size_t *idx, size_t *out);
otto_status_t otto_tensor_get(const otto_tensor_t *mat, const size_t *idx, void *out);
otto_status_t otto_tensor_set(otto_tensor_t *mat, const size_t *idx, const void *src);

int8_t   otto_tensor_get_i8(const otto_tensor_t *mat, const size_t *idx);
int16_t  otto_tensor_get_i16(const otto_tensor_t *mat, const size_t *idx);
int32_t  otto_tensor_get_i32(const otto_tensor_t *mat, const size_t *idx);
int64_t  otto_tensor_get_i64(const otto_tensor_t *mat, const size_t *idx);
uint8_t  otto_tensor_get_u8(const otto_tensor_t *mat, const size_t *idx);
uint16_t otto_tensor_get_u16(const otto_tensor_t *mat, const size_t *idx);
uint32_t otto_tensor_get_u32(const otto_tensor_t *mat, const size_t *idx);
uint64_t otto_tensor_get_u64(const otto_tensor_t *mat, const size_t *idx);
float    otto_tensor_get_f(const otto_tensor_t *mat, const size_t *idx);
double   otto_tensor_get_d(const otto_tensor_t *mat, const size_t *idx);
void     otto_tensor_set_i8(otto_tensor_t *mat, const size_t *idx, const int8_t src);
void     otto_tensor_set_i16(otto_tensor_t *mat, const size_t *idx, const int16_t src);
void     otto_tensor_set_i32(otto_tensor_t *mat, const size_t *idx, const int32_t src);
void     otto_tensor_set_i64(otto_tensor_t *mat, const size_t *idx, const int64_t src);
void     otto_tensor_set_u8(otto_tensor_t *mat, const size_t *idx, const uint8_t src);
void     otto_tensor_set_u16(otto_tensor_t *mat, const size_t *idx, const uint16_t src);
void     otto_tensor_set_u32(otto_tensor_t *mat, const size_t *idx, const uint32_t src);
void     otto_tensor_set_u64(otto_tensor_t *mat, const size_t *idx, const uint64_t src);
void     otto_tensor_set_f(otto_tensor_t *mat, const size_t *idx, const float src);
void     otto_tensor_set_d(otto_tensor_t *mat, const size_t *idx, const double src);

#ifdef __cplusplus
}
#endif
