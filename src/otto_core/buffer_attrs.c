#include <otto/buffer.h>
#include <otto/status.h>
#include <otto_utils/vendor/log.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

otto_status_t otto_buffer_get(const otto_buffer_t *buf, const size_t i, void *out) {
  if (buf->data == NULL || i >= buf->len || out == NULL) {
    return OTTO_STATUS_FAILURE("Invalid data or index");
  }
  memcpy(out, (char *)buf->data + i * buf->data_size, buf->data_size);
  return OTTO_STATUS_SUCCESS;
}

otto_status_t otto_buffer_set(otto_buffer_t *buf, const size_t i, const void *src) {
  if (buf->data == NULL || i >= buf->len || src == NULL) {
    return OTTO_STATUS_FAILURE("Invalid data or index");
  }
  memcpy((char *)buf->data + i * buf->data_size, src, buf->data_size);
  return OTTO_STATUS_SUCCESS;
}

otto_status_t otto_buffer_resize(otto_buffer_t *buf, const size_t new_capacity) {
  void *data = realloc(buf->data, new_capacity * buf->data_size);
  if (data == NULL) {
    return OTTO_STATUS_FAILURE("Could not reallocate data");
  }

  if (new_capacity < buf->len) {
    buf->len = new_capacity;
  }
  buf->capacity = new_capacity;
  buf->data     = data;
  return OTTO_STATUS_SUCCESS;
}

otto_status_t otto_buffer_push(otto_buffer_t *buf, const void *src) {
  if (src == NULL) {
    return OTTO_STATUS_FAILURE("src points to invalid data");
  }

  if (buf->len == buf->capacity) {
    // The buffer is full so we have to reallocate
    void *new_data = realloc(buf->data, (buf->capacity + 1) * buf->data_size);
    if (new_data == NULL) {
      return OTTO_STATUS_FAILURE("Could not reallocate data");
    }
    memcpy((char *)new_data + buf->len * buf->data_size, src, buf->data_size);
    buf->data = new_data;
    buf->len++;
    buf->capacity++;
  } else {
    // It can be assumed that buf->len < buf->capacity, so this
    // means we dont have to allocate more memory.
    memcpy((char *)buf->data + buf->len * buf->data_size, src, buf->data_size);
    buf->len++;
  }
  return OTTO_STATUS_SUCCESS;
}

otto_status_t otto_buffer_extend_array(otto_buffer_t *buf, const void *src, const size_t len) {
  if (src == NULL) {
    return OTTO_STATUS_FAILURE("src points to invalid data");
  }

  uint32_t delta = len - (buf->capacity - buf->len);
  if (delta > 0) {
    // The buffer does not have enough space so we have to allocate more
    // memory
    logi_info("Allocating memory for extension of buffer");
    logi_debug("Allocating %d more bytes", delta);
    void *new_data = realloc(buf->data, (buf->capacity + delta) * buf->data_size);
    if (new_data == NULL) {
      logi_error("Reallocation failed");
      return OTTO_STATUS_FAILURE("Could not reallocate data");
    }
    logi_debug("Copying new memory");
    memcpy((char *)new_data + buf->len * buf->data_size, src, len * buf->data_size);
    buf->data      = new_data;
    buf->len      += len;
    buf->capacity += delta;
    logi_debug("New len is %d", buf->len);
    logi_debug("New capacity is %d", buf->capacity);
  } else {
    // It can be assumed that buf->len < buf->capacity, so this
    // means we dont have to allocate more memory.
    memcpy((char *)buf->data + buf->len * buf->data_size, src, len * buf->data_size);
    buf->len += len;
  }
  return OTTO_STATUS_SUCCESS;
}

#define _OTTO_BUFFER_DEFINE_GET(T, t)                                                                                                                \
  T otto_buffer_get_##t(const otto_buffer_t *buf, const size_t i) {                                                                                  \
    T out;                                                                                                                                           \
    (void)otto_buffer_get(buf, i, &out);                                                                                                             \
    return out;                                                                                                                                      \
  }
#define _OTTO_BUFFER_DEFINE_SET(T, t)                                                                                                                \
  void otto_buffer_set_##t(otto_buffer_t *buf, const size_t i, const T src) { (void)otto_buffer_set(buf, i, &src); }
#define _OTTO_BUFFER_DEFINE_PUSH(T, t)                                                                                                               \
  void otto_buffer_push_##t(otto_buffer_t *buf, const T src) { (void)otto_buffer_push(buf, &src); }

_OTTO_BUFFER_DEFINE_GET(int8_t, i8)
_OTTO_BUFFER_DEFINE_GET(int16_t, i16)
_OTTO_BUFFER_DEFINE_GET(int32_t, i32)
_OTTO_BUFFER_DEFINE_GET(int64_t, i64)
_OTTO_BUFFER_DEFINE_GET(uint8_t, u8)
_OTTO_BUFFER_DEFINE_GET(uint16_t, u16)
_OTTO_BUFFER_DEFINE_GET(uint32_t, u32)
_OTTO_BUFFER_DEFINE_GET(uint64_t, u64)
_OTTO_BUFFER_DEFINE_GET(float, f)
_OTTO_BUFFER_DEFINE_GET(double, d)

_OTTO_BUFFER_DEFINE_SET(int8_t, i8)
_OTTO_BUFFER_DEFINE_SET(int16_t, i16)
_OTTO_BUFFER_DEFINE_SET(int32_t, i32)
_OTTO_BUFFER_DEFINE_SET(int64_t, i64)
_OTTO_BUFFER_DEFINE_SET(uint8_t, u8)
_OTTO_BUFFER_DEFINE_SET(uint16_t, u16)
_OTTO_BUFFER_DEFINE_SET(uint32_t, u32)
_OTTO_BUFFER_DEFINE_SET(uint64_t, u64)
_OTTO_BUFFER_DEFINE_SET(float, f)
_OTTO_BUFFER_DEFINE_SET(double, d)

_OTTO_BUFFER_DEFINE_PUSH(int8_t, i8)
_OTTO_BUFFER_DEFINE_PUSH(int16_t, i16)
_OTTO_BUFFER_DEFINE_PUSH(int32_t, i32)
_OTTO_BUFFER_DEFINE_PUSH(int64_t, i64)
_OTTO_BUFFER_DEFINE_PUSH(uint8_t, u8)
_OTTO_BUFFER_DEFINE_PUSH(uint16_t, u16)
_OTTO_BUFFER_DEFINE_PUSH(uint32_t, u32)
_OTTO_BUFFER_DEFINE_PUSH(uint64_t, u64)
_OTTO_BUFFER_DEFINE_PUSH(float, f)
_OTTO_BUFFER_DEFINE_PUSH(double, d)
