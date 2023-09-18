#include <stddef.h>
#include <stdint.h>

#include <otto/devices.h>
#include <otto/status.h>
#include <otto/vector.h>
#include <otto_utils/test.h>
#include <ottou/log.h>
#include <ottou/macros.h>

int test_new() {
  otto_vector_t vec;

  log_debug("Creating vector");
  CALL(otto_vector_new(sizeof(uint32_t), &vec), "Failed creating vector");

  log_debug("Checking creation result");
  OTTO_ASSERT_EQI(vec.data, NULL);
  OTTO_ASSERT_EQI(vec.capacity, 0);
  OTTO_ASSERT_EQI(vec.device, OTTO_DEVICE_CPU);
  OTTO_ASSERT_EQI(vec.len, 0);
  return TEST_PASS;
}

int test_zero() {
  otto_vector_t vec;
  size_t len = 10;

  log_info("Creating vector");
  CALL(otto_vector_zero(len, sizeof(uint32_t), &vec), "Failed creating vector");

  log_debug("Checking creation result");
  for (int i = 0; i < len; i++) {
    OTTO_ASSERT_EQI(*(uint32_t *)(vec.data + i * sizeof(uint32_t)), 0);
  }
  OTTO_ASSERT_EQI(vec.device, OTTO_DEVICE_CPU);
  return TEST_PASS;
}

int test_with_capacity() {
  otto_vector_t vec;

  log_debug("Creating vector");
  CALL(otto_vector_with_capacity(6, sizeof(uint32_t), &vec),
       "Failed creating vector");

  log_debug("Checking creation result");
  OTTO_ASSERT_NEI(vec.data, NULL);
  OTTO_ASSERT_NEI(vec.capacity, 0);
  OTTO_ASSERT_EQI(vec.data_size, sizeof(uint32_t));
  OTTO_ASSERT_EQI(vec.device, OTTO_DEVICE_CPU);
  OTTO_ASSERT_EQI(vec.len, 0);
  return TEST_PASS;
}

int test_from_array() {
  otto_vector_t vec;
  uint32_t data[] = {0, 1, 2, 3, 4, 5, 6, 7};
  size_t len = 8;

  log_info("Creating vector");
  CALL(otto_vector_from_array(data, len, sizeof(uint32_t), &vec),
       "Failed creating vector");

  log_debug("Checking creation result");
  for (int i = 0; i < len; i++) {
    OTTO_ASSERT_EQI(*(uint32_t *)(vec.data + i * sizeof(uint32_t)), data[i]);
  }
  OTTO_ASSERT_EQI(vec.device, OTTO_DEVICE_CPU);

  return TEST_PASS;
}

int main() {
  OTTO_CALL_TEST(test_new);
  OTTO_CALL_TEST(test_zero);
  OTTO_CALL_TEST(test_with_capacity);
  OTTO_CALL_TEST(test_from_array);
  return TEST_PASS;
}
