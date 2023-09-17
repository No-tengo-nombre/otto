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

otto_status_t otto_vector_push(otto_vector_t *vec, const void *src) {
  if (src == NULL) {
    return OTTO_STATUS_FAILURE;
  }

  if (vec->len == vec->capacity) {
    // The vector is full so we have to reallocate
    void *new_data = realloc(vec->data, vec->capacity + 1);
    if (new_data == NULL) {
      free(vec->data);
      return OTTO_STATUS_FAILURE;
    }
    memcpy(new_data + vec->len * vec->data_size, src, vec->data_size);
    vec->data = new_data;
    vec->len++;
    vec->capacity++;
  } else {
    // It can be assumed that vec->len < vec->capacity, so this
    // means we dont have to allocate more memory.
    memcpy(vec->data + vec->len * vec->data_size, src, vec->data_size);
  }
  return OTTO_STATUS_SUCCESS;
}

otto_status_t otto_vector_extend_array(otto_vector_t *vec, const void *src,
                                       const size_t len) {
  if (src == NULL) {
    return OTTO_STATUS_FAILURE;
  }

  size_t delta = len - (vec->capacity - vec->len);
  if (delta > 0) {
    // The vector does not have enough space so we have to allocate more
    // memory
    void *new_data = realloc(vec->data, vec->capacity + delta);
    if (new_data == NULL) {
      free(vec->data);
      return OTTO_STATUS_FAILURE;
    }
    memcpy(new_data + vec->len * vec->data_size, src, len * vec->data_size);
    vec->data = new_data;
    vec->len += len;
    vec->capacity += delta;
  } else {
    // It can be assumed that vec->len < vec->capacity, so this
    // means we dont have to allocate more memory.
    memcpy(vec->data + vec->len * vec->data_size, src, len * vec->data_size);
  }
  return OTTO_STATUS_SUCCESS;
}
