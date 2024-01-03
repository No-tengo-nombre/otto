#pragma once

#include "status.h"
#include "vector.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Tensor object that generalizes a vector to any rank.
 *
 * Tensors are designed to contain otto vectors while providing abstractions
 * that facilitate doing mathematical work with them. They wrap some contiguous
 * data, while containing higher level information about what said data
 * represents (e.g. nine contiguous floats can correspond to a length 9 vector
 * or a 3x3 matrix).
 *
 * The `shape` attribute corresponds to an array of `rank` elements describing
 * the shape of the underlying data.
 */
typedef struct otto_tensor {
  otto_vector_t vec;
  size_t rank;
  size_t *shape;
} otto_tensor_t;

/* Tensor creation */

// otto_status_t otto_tensor_new(otto_tensor_t *out);
otto_status_t otto_tensor_zero(const size_t rank, size_t *shape,
                               const size_t data_size, otto_tensor_t *out);
// otto_status_t otto_tensor_from_vector(otto_vector_t *vec, const size_t rank,
//                                       const int64_t *shape);

/* Tensor attributes */

otto_status_t otto_tensor_get(const otto_tensor_t *mat, const size_t *idx,
                              void *out);
otto_status_t otto_tensor_set(otto_tensor_t *mat, const size_t *idx,
                              const void *src);

#ifdef __cplusplus
}
#endif
