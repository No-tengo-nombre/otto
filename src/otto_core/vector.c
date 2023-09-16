#include <stddef.h>
#include <stdlib.h>

#include <otto/devices.h>
#include <otto/status.h>
#include <otto/vector.h>
#include <string.h>

otto_status_t otto_vector_new(const size_t data_size, otto_vector_t *out) {
  otto_vector_t result = {
      .data = NULL,
      .data_size = data_size,
      .len = 0,
      .capacity = 0,
      .device = OTTO_DEVICE_CPU,
  };
  *out = result;
  return OTTO_STATUS_SUCCESS;
}

otto_status_t otto_vector_zero(const size_t len, const size_t data_size,
                               otto_vector_t *out) {
  void *data = calloc(len, data_size);
  if (data == NULL) {
    return OTTO_STATUS_FAILURE;
  }

  otto_vector_t result = {
      .data = data,
      .data_size = data_size,
      .len = len,
      .capacity = len,
      .device = OTTO_DEVICE_CPU,
  };
  *out = result;
  return OTTO_STATUS_SUCCESS;
}

otto_status_t otto_vector_with_capacity(const size_t capacity,
                                        const size_t data_size,
                                        otto_vector_t *out) {
  void *data = malloc(data_size * capacity);
  if (data == NULL) {
    return OTTO_STATUS_FAILURE;
  }

  otto_vector_t result = {
      .data = data,
      .data_size = data_size,
      .len = 0,
      .capacity = capacity,
      .device = OTTO_DEVICE_CPU,
  };
  *out = result;
  return OTTO_STATUS_SUCCESS;
}

otto_status_t otto_vector_from_array(const void *data, const size_t len,
                                     const size_t data_size,
                                     otto_vector_t *out) {
  void *new_data = malloc(len * data_size);
  if (new_data == NULL) {
    return OTTO_STATUS_FAILURE;
  }

  memcpy(new_data, data, len * data_size);
  otto_vector_t result = {
      .data = new_data,
      .data_size = data_size,
      .len = len,
      .capacity = len,
      .device = OTTO_DEVICE_CPU,
  };
  *out = result;
  return OTTO_STATUS_SUCCESS;
}

otto_status_t otto_vector_cleanup(const otto_vector_t *const vec) {
  free(vec->data);

  return OTTO_STATUS_SUCCESS;
}

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
