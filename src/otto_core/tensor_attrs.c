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
  size_t index = calculate_index(idx, mat->rank, mat->shape);
  if (index < 0 || index >= mat->vec.len) {
    logi_error("Index %i out of bounds", index);
    return OTTO_STATUS_FAILURE("Index out of bounds");
  }
  logi_debug("Setting index %i", index);
  OTTO_CALL_I(otto_vector_set(&mat->vec, index, src),
              "Failed setting index from underlying vector");
  return OTTO_STATUS_SUCCESS;
  // TODO: Test this implementation
}
