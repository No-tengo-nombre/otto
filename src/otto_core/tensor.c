#include <otto/status.h>
#include <otto/tensor.h>
#include <otto/vector.h>
#include <otto_utils/macros.h>
#include <otto_utils/vendor/log.h>

#include <stdint.h>

size_t calculate_index(const size_t *idx, const size_t rank,
                       const size_t *shapes) {
  size_t index = 0;
  for (int k = 0; k < rank; k++) {
    size_t s = 1;
    for (int n = k + 1; n < rank; n++) {
      s *= shapes[n];
    }
    index += s * (*idx++);
  }
  return index;
}

// otto_status_t otto_tensor_new(otto_tensor_t *out) {
//   otto_tensor_t result = {
//       .vec = NULL,
//       .rank = 0,
//       .shape = NULL,
//   };
//   *out = result;
//   return OTTO_STATUS_SUCCESS;
// }

otto_status_t otto_tensor_zero(const size_t rank, size_t *shape,
                               const size_t data_size, otto_tensor_t *out) {
  size_t len = 1;
  size_t *original_shape = shape;
  for (int i = 0; i < rank; i++) {
    len *= *shape++;
  }
  logi_debug("Length for underlying vector is %i", len);
  otto_vector_t vec;
  OTTO_CALL_I(otto_vector_zero(len, data_size, &vec), "Failed creating vector");
  otto_tensor_t result = {
      .vec = vec,
      .rank = rank,
      .shape = original_shape,
  };
  *out = result;
  return OTTO_STATUS_SUCCESS;
}

otto_status_t otto_tensor_get(const otto_tensor_t *mat, const size_t *idx,
                              void *out) {
  size_t index = calculate_index(idx, mat->rank, mat->shape);
  if (index < 0 || index >= mat->vec.len) {
    logi_error("Index %i out of bounds", index);
    return OTTO_STATUS_FAILURE("Index out of bounds");
  }
  logi_debug("Getting index %i", index);
  OTTO_CALL_I(otto_vector_get(&mat->vec, index, out),
              "Failed getting index from underlying vector");
  return OTTO_STATUS_SUCCESS;
}

otto_status_t otto_tensor_set(otto_tensor_t *mat, const size_t *idx,
                              const void *src) {
  return OTTO_STATUS_FAILURE("Not implemented");
}
