#include <otto/buffer.h>
#include <otto/status.h>
#include <otto/tensor.h>
#include <otto_utils/macros.h>
#include <otto_utils/vendor/log.h>
#include <stdint.h>

otto_status_t otto_tensor_calculate_index(const otto_tensor_t *mat, const size_t *idx, size_t *out) {
  size_t index = 0;
  for (int k = 0; k < mat->rank; k++) {
    size_t s = 1;
    for (int n = k + 1; n < mat->rank; n++) {
      s *= mat->shape[n];
    }
    index += s * (*idx++);
  }
  if (index < 0 || index >= mat->buf.len) {
    logi_error("Index %i out of bounds", index);
    return OTTO_STATUS_FAILURE("Index out of bounds");
  }
  *out = index;
  return OTTO_STATUS_SUCCESS;
}

otto_status_t otto_tensor_get(const otto_tensor_t *mat, const size_t *idx, void *out) {
  size_t index;
  OTTO_CALL_I(otto_tensor_calculate_index(mat, idx, &index), "Index out of bounds");
  logi_debug("Getting index %i", index);
  OTTO_CALL_I(otto_buffer_get(&mat->buf, index, out), "Failed getting index from underlying buffer");
  return OTTO_STATUS_SUCCESS;
}

otto_status_t otto_tensor_set(otto_tensor_t *mat, const size_t *idx, const void *src) {
  size_t index;
  OTTO_CALL_I(otto_tensor_calculate_index(mat, idx, &index), "Index out of bounds");
  logi_debug("Setting index %i", index);
  OTTO_CALL_I(otto_buffer_set(&mat->buf, index, src), "Failed setting index from underlying buffer");
  return OTTO_STATUS_SUCCESS;
}

#define _OTTO_TENSOR_DEFINE_GET(T, t)                                                                                                                \
  T otto_tensor_get_##t(const otto_tensor_t *mat, const size_t *idx) {                                                                               \
    T out;                                                                                                                                           \
    (void)otto_tensor_get(mat, idx, &out);                                                                                                           \
    return out;                                                                                                                                      \
  }

#define _OTTO_TENSOR_DEFINE_SET(T, t)                                                                                                                \
  void otto_tensor_set_##t(otto_tensor_t *mat, const size_t *idx, const T src) { (void)otto_tensor_set(mat, idx, &src); }

_OTTO_TENSOR_DEFINE_GET(int8_t, i8)
_OTTO_TENSOR_DEFINE_GET(int16_t, i16)
_OTTO_TENSOR_DEFINE_GET(int32_t, i32)
_OTTO_TENSOR_DEFINE_GET(int64_t, i64)
_OTTO_TENSOR_DEFINE_GET(uint8_t, u8)
_OTTO_TENSOR_DEFINE_GET(uint16_t, u16)
_OTTO_TENSOR_DEFINE_GET(uint32_t, u32)
_OTTO_TENSOR_DEFINE_GET(uint64_t, u64)
_OTTO_TENSOR_DEFINE_GET(float, f)
_OTTO_TENSOR_DEFINE_GET(double, d)

_OTTO_TENSOR_DEFINE_SET(int8_t, i8)
_OTTO_TENSOR_DEFINE_SET(int16_t, i16)
_OTTO_TENSOR_DEFINE_SET(int32_t, i32)
_OTTO_TENSOR_DEFINE_SET(int64_t, i64)
_OTTO_TENSOR_DEFINE_SET(uint8_t, u8)
_OTTO_TENSOR_DEFINE_SET(uint16_t, u16)
_OTTO_TENSOR_DEFINE_SET(uint32_t, u32)
_OTTO_TENSOR_DEFINE_SET(uint64_t, u64)
_OTTO_TENSOR_DEFINE_SET(float, f)
_OTTO_TENSOR_DEFINE_SET(double, d)
