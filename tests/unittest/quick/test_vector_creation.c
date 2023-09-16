#include <stddef.h>
#include <stdint.h>

#include <otto/status.h>
#include <otto/vector.h>

#include "otto/devices.h"
#include "ottou/log.h"
#include "src/otto_utils/test.h"

int test_new() {
  log_info("Testing `new`");
  otto_vector_t vec;
  otto_status_t status = OTTO_STATUS_SUCCESS;

  log_debug("Creating vector");
  status &= otto_vector_new(sizeof(uint32_t), &vec);

  log_debug("Checking creation result");
  if (status == OTTO_STATUS_FAILURE) {
    return TEST_FAIL;
  }
  if (vec.data != NULL) {
    return TEST_FAIL;
  }
  if (vec.capacity != 0) {
    return TEST_FAIL;
  }
  if (vec.device != OTTO_DEVICE_CPU) {
    return TEST_FAIL;
  }
  if (vec.size != 0) {
    return TEST_FAIL;
  }
  return TEST_PASS;
}

int test_zero() {
  log_info("Testing `zero`");
  otto_vector_t vec;
  otto_status_t status = OTTO_STATUS_SUCCESS;
  size_t len = 10;

  log_info("Creating vector");
  status &= otto_vector_zero(len, sizeof(uint32_t), &vec);

  log_debug("Checking creation result");
  if (status == OTTO_STATUS_FAILURE) {
    return TEST_FAIL;
  }
  // Check one by one that the elements in the vector are correct
  for (int i = 0; i < len; i++) {
    if (*(uint32_t *)(vec.data + i * sizeof(uint32_t)) != 0) {
      return TEST_FAIL;
    }
  }
  if (vec.device != OTTO_DEVICE_CPU) {
    return TEST_FAIL;
  }

  return TEST_PASS;
}

int test_with_capacity() {
  log_info("Testing `with_capacity`");
  otto_vector_t vec;
  otto_status_t status = OTTO_STATUS_SUCCESS;

  log_debug("Creating vector");
  status &= otto_vector_with_capacity(6, sizeof(uint32_t), &vec);

  log_debug("Checking creation result");
  if (status == OTTO_STATUS_FAILURE) {
    return TEST_FAIL;
  }
  if (vec.data == NULL) {
    return TEST_FAIL;
  }
  if (vec.capacity == 0) {
    return TEST_FAIL;
  }
  if (vec.data_size != sizeof(uint32_t)) {
    return TEST_FAIL;
  }
  if (vec.device != OTTO_DEVICE_CPU) {
    return TEST_FAIL;
  }
  if (vec.size != 0) {
    return TEST_FAIL;
  }
  return TEST_PASS;
}

int test_from_array() {
  log_info("Testing `from_array`");
  otto_vector_t vec;
  uint32_t data[] = {0, 1, 2, 3, 4, 5, 6, 7};
  size_t len = 8;
  otto_status_t status = OTTO_STATUS_SUCCESS;

  log_info("Creating vector");
  status &= otto_vector_from_array(data, len, sizeof(uint32_t), &vec);

  log_debug("Checking creation result");
  if (status == OTTO_STATUS_FAILURE) {
    return TEST_FAIL;
  }
  // Check one by one that the elements in the vector are correct
  for (int i = 0; i < len; i++) {
    if (data[i] != *(uint32_t *)(vec.data + i * sizeof(uint32_t))) {
      return TEST_FAIL;
    }
  }
  if (vec.device != OTTO_DEVICE_CPU) {
    return TEST_FAIL;
  }

  return TEST_PASS;
}

int main() {
  OTTO_CALL_TEST(test_new);
  OTTO_CALL_TEST(test_zero);
  OTTO_CALL_TEST(test_with_capacity);
  OTTO_CALL_TEST(test_from_array);
  return TEST_PASS;
}
