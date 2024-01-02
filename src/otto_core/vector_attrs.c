#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <otto/status.h>
#include <otto/vector.h>
#include <otto_utils/vendor/log.h>

otto_status_t otto_vector_get(const otto_vector_t *vec, const size_t i,
                              void *out) {
  if (vec->data == NULL || i >= vec->len || out == NULL) {
    return OTTO_STATUS_FAILURE("Invalid data or index");
  }
  memcpy(out, (char *)vec->data + i * vec->data_size, vec->data_size);
  return OTTO_STATUS_SUCCESS;
}

otto_status_t otto_vector_set(otto_vector_t *vec, const size_t i,
                              const void *src) {
  if (vec->data == NULL || i >= vec->len || src == NULL) {
    return OTTO_STATUS_FAILURE("Invalid data or index");
  }
  memcpy((char *)vec->data + i * vec->data_size, src, vec->data_size);
  return OTTO_STATUS_SUCCESS;
}

otto_status_t otto_vector_resize(otto_vector_t *vec,
                                 const size_t new_capacity) {
  void *data = realloc(vec->data, new_capacity * vec->data_size);
  if (data == NULL) {
    return OTTO_STATUS_FAILURE("Could not reallocate data");
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
    return OTTO_STATUS_FAILURE("src points to invalid data");
  }

  if (vec->len == vec->capacity) {
    // The vector is full so we have to reallocate
    void *new_data = realloc(vec->data, (vec->capacity + 1) * vec->data_size);
    if (new_data == NULL) {
      return OTTO_STATUS_FAILURE("Could not reallocate data");
    }
    memcpy((char *)new_data + vec->len * vec->data_size, src, vec->data_size);
    vec->data = new_data;
    vec->len++;
    vec->capacity++;
  } else {
    // It can be assumed that vec->len < vec->capacity, so this
    // means we dont have to allocate more memory.
    memcpy((char *)vec->data + vec->len * vec->data_size, src, vec->data_size);
    vec->len++;
  }
  return OTTO_STATUS_SUCCESS;
}

otto_status_t otto_vector_extend_array(otto_vector_t *vec, const void *src,
                                       const size_t len) {
  if (src == NULL) {
    return OTTO_STATUS_FAILURE("src points to invalid data");
  }

  uint32_t delta = len - (vec->capacity - vec->len);
  if (delta > 0) {
    // The vector does not have enough space so we have to allocate more
    // memory
    logi_info("Allocating memory for extension of vector");
    logi_debug("Allocating %d more bytes", delta);
    void *new_data =
        realloc(vec->data, (vec->capacity + delta) * vec->data_size);
    if (new_data == NULL) {
      logi_error("Reallocation failed");
      return OTTO_STATUS_FAILURE("Could not reallocate data");
    }
    logi_debug("Copying new memory");
    memcpy((char *)new_data + vec->len * vec->data_size, src,
           len * vec->data_size);
    vec->data = new_data;
    vec->len += len;
    vec->capacity += delta;
    logi_debug("New len is %d", vec->len);
    logi_debug("New capacity is %d", vec->capacity);
  } else {
    // It can be assumed that vec->len < vec->capacity, so this
    // means we dont have to allocate more memory.
    memcpy((char *)vec->data + vec->len * vec->data_size, src,
           len * vec->data_size);
    vec->len += len;
  }
  return OTTO_STATUS_SUCCESS;
}
