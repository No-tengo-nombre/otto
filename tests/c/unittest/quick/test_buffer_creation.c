
#include <otto/buffer.h>
#include <otto/devices.h>
#include <otto/status.h>
#include <otto_utils/test.h>
#include <ottou/log.h>
#include <ottou/macros.h>
#include <stddef.h>
#include <stdint.h>

otto_status_t test_new(void) {
  otto_buffer_t buf;

  log_debug("Creating buffer");
  OTTO_CALL(otto_buffer_new(sizeof(uint32_t), &buf), "Failed creating buffer");

  log_debug("Checking creation result");
  OTTO_ASSERT_EQI(buf.data, NULL);
  OTTO_ASSERT_EQI(buf.capacity, 0);
  OTTO_ASSERT_EQI(buf.device, OTTO_DEVICE_CPU);
  OTTO_ASSERT_EQI(buf.len, 0);
  return OTTO_STATUS_SUCCESS;
}

otto_status_t test_zero(void) {
  otto_buffer_t buf;
  size_t        len = 10;

  log_info("Creating buffer");
  OTTO_CALL(otto_buffer_zero(len, sizeof(uint32_t), &buf), "Failed creating buffer");

  log_debug("Checking creation result");
  for (uint32_t i = 0; i < len; i++) {
    OTTO_ASSERT_EQI(*(uint32_t *)((char *)buf.data + i * sizeof(uint32_t)), 0);
  }
  OTTO_ASSERT_EQI(buf.device, OTTO_DEVICE_CPU);
  return OTTO_STATUS_SUCCESS;
}

otto_status_t test_with_capacity(void) {
  otto_buffer_t buf;

  log_debug("Creating buffer");
  OTTO_CALL(otto_buffer_with_capacity(6, sizeof(uint32_t), &buf), "Failed creating buffer");

  log_debug("Checking creation result");
  OTTO_ASSERT_NEI(buf.data, NULL);
  OTTO_ASSERT_NEI(buf.capacity, 0);
  OTTO_ASSERT_EQI(buf.data_size, sizeof(uint32_t));
  OTTO_ASSERT_EQI(buf.device, OTTO_DEVICE_CPU);
  OTTO_ASSERT_EQI(buf.len, 0);
  return OTTO_STATUS_SUCCESS;
}

otto_status_t test_from_array(void) {
  otto_buffer_t buf;
  uint32_t      data[] = {0, 1, 2, 3, 4, 5, 6, 7};
  size_t        len    = 8;

  log_info("Creating buffer");
  OTTO_CALL(otto_buffer_from_array(data, len, sizeof(uint32_t), &buf), "Failed creating buffer");

  log_debug("Checking creation result");
  for (uint32_t i = 0; i < len; i++) {
    OTTO_ASSERT_EQI(*(uint32_t *)((char *)buf.data + i * sizeof(uint32_t)), data[i]);
  }
  OTTO_ASSERT_EQI(buf.device, OTTO_DEVICE_CPU);

  return OTTO_STATUS_SUCCESS;
}

int main(void) {
  OTTO_CALL_TEST(test_new);
  OTTO_CALL_TEST(test_zero);
  OTTO_CALL_TEST(test_with_capacity);
  OTTO_CALL_TEST(test_from_array);

  return TEST_PASS;
}
