#include <stddef.h>
#include <stdlib.h>

#include <otto/devices.h>
#include <otto/status.h>
#include <otto/vector.h>
#include <string.h>

otto_status_t otto_vector_new(otto_vector_t *out) {
  otto_vector_t result = {
      .data = NULL,
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

  memcpy(new_data, data, size);
  otto_vector_t result = {
      .data = new_data,
      .size = size,
      .capacity = size,
      .device = OTTO_DEVICE_CPU,
  };
  *out = result;
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
