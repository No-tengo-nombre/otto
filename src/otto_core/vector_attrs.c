#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <otto/status.h>
#include <otto/vector.h>

otto_status_t otto_vector_get(const otto_vector_t *vec, const size_t i,
                              void *out) {
  if (vec->data == NULL || i >= vec->len || out == NULL) {
    return OTTO_STATUS_FAILURE;
  }
  memcpy(out, vec->data + i * vec->data_size, vec->data_size);
  return OTTO_STATUS_SUCCESS;
}

otto_status_t otto_vector_set(otto_vector_t *vec, const size_t i,
                              const void *src) {
  if (vec->data == NULL || i >= vec->len || src == NULL) {
    return OTTO_STATUS_FAILURE;
  }
  memcpy(vec->data + i * vec->data_size, src, vec->data_size);
  return OTTO_STATUS_SUCCESS;
}

otto_status_t otto_vector_resize(otto_vector_t *vec,
                                 const size_t new_capacity) {
  void *data = realloc(vec->data, new_capacity);
  if (data == NULL) {
    return OTTO_STATUS_FAILURE;
  }

  if (new_capacity < vec->len) {
    vec->len = new_capacity;
  }
  vec->capacity = new_capacity;
  vec->data = data;
  return OTTO_STATUS_SUCCESS;
}

otto_status_t otto_vector_push(const void *src, otto_vector_t *out) {
  if (src == NULL) {
    return OTTO_STATUS_FAILURE;
  }

  if (out->len == out->capacity) {
    // The vector is full so we have to reallocate
    void *new_data = realloc(out->data, out->capacity + 1);
    if (new_data == NULL) {
      free(out->data);
      return OTTO_STATUS_FAILURE;
    }
    memcpy(new_data + out->len * out->data_size, src, out->data_size);
    out->data = new_data;
    out->len++;
    out->capacity++;
  } else {
    // It can be assumed that out->len < out->capacity, so this
    // means we dont have to allocate more memory.
    memcpy(out->data + out->len * out->data_size, src, out->data_size);
  }
  return OTTO_STATUS_SUCCESS;
}

otto_status_t otto_vector_extend_array(const void *src, const size_t len,
                                       otto_vector_t *out) {
  if (src == NULL) {
    return OTTO_STATUS_FAILURE;
  }

  size_t delta = len - (out->capacity - out->len);
  if (delta > 0) {
    // The vector does not have enough space so we have to allocate more
    // memory
    void *new_data = realloc(out->data, out->capacity + delta);
    if (new_data == NULL) {
      free(out->data);
      return OTTO_STATUS_FAILURE;
    }
    memcpy(new_data + out->len * out->data_size, src, len * out->data_size);
    out->data = new_data;
    out->len += len;
    out->capacity += delta;
  } else {
    // It can be assumed that out->len < out->capacity, so this
    // means we dont have to allocate more memory.
    memcpy(out->data + out->len * out->data_size, src, len * out->data_size);
  }
  return OTTO_STATUS_SUCCESS;
}
