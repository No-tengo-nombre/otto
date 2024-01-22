#include <otto/status.h>
#include <otto/tensor.h>
#include <otto/buffer.h>
#include <otto_utils/macros.h>
#include <otto_utils/vendor/log.h>

#include <stdint.h>

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
  logi_debug("Length for underlying buffer is %i", len);
  otto_buffer_t vec;
  OTTO_CALL_I(otto_buffer_zero(len, data_size, &vec), "Failed creating buffer");
  otto_tensor_t result = {
      .vec = vec,
      .rank = rank,
      .shape = original_shape,
  };
  *out = result;
  return OTTO_STATUS_SUCCESS;
}

otto_status_t otto_tensor_from_array(const void *data, const size_t rank,
                                     size_t *shape, const size_t data_size,
                                     otto_tensor_t *out) {
  return OTTO_STATUS_FAILURE("Not implemented");
}

otto_status_t otto_tensor_cleanup(const otto_tensor_t *const mat) {
  OTTO_CALL_I(otto_buffer_cleanup(&mat->vec),
              "Failed cleaning up underlying buffer");
  return OTTO_STATUS_SUCCESS;
}
