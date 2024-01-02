#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <otto/cl/cl.h>
#include <otto/status.h>
#include <otto/vector.h>
#include <otto_utils/macros.h>

otto_status_t otto_vector_new(const size_t data_size, otto_vector_t *out) {
  otto_vector_t result = {
      .data = NULL,
      .gmem = NULL,
      .data_size = data_size,
      .len = 0,
      .capacity = 0,
      .device = OTTO_DEVICE_CPU,
      .ctx = NULL,
      .flags = CL_MEM_READ_ONLY,
  };
  *out = result;
  return OTTO_SUCCESS;
}

otto_status_t otto_vector_zero(const size_t len, const size_t data_size,
                               otto_vector_t *out) {
  void *data = calloc(len, data_size);
  if (data == NULL) {
    return OTTO_FAILURE;
  }

  otto_vector_t result = {
      .data = data,
      .gmem = NULL,
      .data_size = data_size,
      .len = len,
      .capacity = len,
      .device = OTTO_DEVICE_CPU,
      .ctx = NULL,
      .flags = CL_MEM_READ_ONLY,
  };
  *out = result;
  return OTTO_SUCCESS;
}

otto_status_t otto_vector_with_capacity(const size_t capacity,
                                        const size_t data_size,
                                        otto_vector_t *out) {
  void *data = malloc(data_size * capacity);
  if (data == NULL) {
    return OTTO_FAILURE;
  }

  otto_vector_t result = {
      .data = data,
      .gmem = NULL,
      .data_size = data_size,
      .len = 0,
      .capacity = capacity,
      .device = OTTO_DEVICE_CPU,
      .ctx = NULL,
      .flags = CL_MEM_READ_ONLY,
  };
  *out = result;
  return OTTO_SUCCESS;
}

otto_status_t otto_vector_from_array(const void *data, const size_t len,
                                     const size_t data_size,
                                     otto_vector_t *out) {
  void *new_data = malloc(len * data_size);
  if (new_data == NULL) {
    return OTTO_FAILURE;
  }

  memcpy(new_data, data, len * data_size);
  otto_vector_t result = {
      .data = new_data,
      .gmem = NULL,
      .data_size = data_size,
      .len = len,
      .capacity = len,
      .device = OTTO_DEVICE_CPU,
      .ctx = NULL,
      .flags = CL_MEM_READ_ONLY,
  };
  *out = result;
  return OTTO_SUCCESS;
}

otto_status_t otto_vector_cleanup(const otto_vector_t *const vec) {
  free(vec->data);

  if (vec->gmem != NULL) {
    OTTO_CL_CALL_I(clReleaseMemObject(vec->gmem),
                   "Failed releasing device memory");
  }

  return OTTO_SUCCESS;
}
