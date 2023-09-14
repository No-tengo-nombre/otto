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
      .size = 0,
      .capacity = 0,
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
      .size = 0,
      .capacity = capacity,
      .device = OTTO_DEVICE_CPU,
  };
  *out = result;
  return OTTO_STATUS_SUCCESS;
}

otto_status_t otto_vector_from_array(const void *data, const size_t size,
                                     const size_t data_size,
                                     otto_vector_t *out) {
  void *new_data = malloc(size * data_size);
  if (new_data == NULL) {
    return OTTO_STATUS_FAILURE;
  }

  memcpy(new_data, data, size * data_size);
  otto_vector_t result = {
      .data = new_data,
      .data_size = data_size,
      .size = size,
      .capacity = size,
      .device = OTTO_DEVICE_CPU,
  };
  *out = result;
  return OTTO_STATUS_SUCCESS;
}

otto_status_t otto_vector_cleanup(const otto_vector_t *const vec) {
  free(vec->data);

  return OTTO_STATUS_SUCCESS;
}

otto_status_t otto_vector_get(const otto_vector_t *vec, const size_t i, void *out) {
  if (vec->data == NULL || i >= vec->size || out == NULL) {
    return OTTO_STATUS_FAILURE;
  }
  memcpy(out, vec->data + i * vec->data_size, vec->data_size);
  return OTTO_STATUS_SUCCESS;
}

otto_status_t otto_vector_push(const void *target, otto_vector_t *out) {
  // TODO: Implement pushing to the vector
  return OTTO_STATUS_FAILURE;
}
otto_status_t otto_vector_extend_array(const void *target, const size_t size,
                                       otto_vector_t *out) {
  // TODO: Implement extending the vector
  return OTTO_STATUS_FAILURE;
}
