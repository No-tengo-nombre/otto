#include <otto/buffer.h>
#include <otto/devices.h>
#include <otto/status.h>
#include <otto_utils/test.h>
#include <ottou/log.h>
#include <ottou/macros.h>
#include <stddef.h>
#include <stdint.h>

otto_status_t test_modification_push1(void) {
  otto_buffer_t buf;
  uint32_t      data[] = {0, 1, 2, 3, 4, 5, 6, 7};
  size_t        len    = 8;

  log_debug("Creating buffer");
  OTTO_CALL(otto_buffer_from_array(data, len, sizeof(uint32_t), &buf), "Failed creating buffer");

  log_debug("Puhsing element");
  uint32_t val = 20;
  OTTO_CALL(otto_buffer_push(&buf, &val), "Failed pushing element");

  log_debug("Checking that the dimensions changed properly");
  OTTO_ASSERT_EQI(buf.len, 9);
  OTTO_ASSERT_EQI(buf.capacity, 9);

  log_debug("Checking border scenario");
  if (otto_buffer_push(&buf, NULL).status != OTTO_FAILURE) {
    log_fatal("Pushing NULL did not fail");
    return OTTO_STATUS_FAILURE("Expected fail pushing element");
  }
  log_debug("Checking that the failed push does not modify the buffer");
  OTTO_ASSERT_EQI(buf.len, 9);
  OTTO_ASSERT_EQI(buf.capacity, 9);

  log_debug("Checking that the push happened fine");
  uint32_t contained_val;
  OTTO_CALL(otto_buffer_get(&buf, 8, &contained_val), "Failed getting buf[8]");
  OTTO_ASSERT_EQI(contained_val, val);
  return OTTO_STATUS_SUCCESS;
}

otto_status_t test_modification_push2(void) {
  otto_buffer_t buf;
  size_t        capacity = 8;

  log_debug("Creating buffer");
  OTTO_CALL(otto_buffer_with_capacity(capacity, sizeof(uint32_t), &buf), "Failed creating buffer");

  log_debug("Pushing element");
  uint32_t val = 20;
  OTTO_CALL(otto_buffer_push(&buf, &val), "Failed pushing element");

  log_debug("Checking that the dimensions changed properly");
  OTTO_ASSERT_EQI(buf.len, 1);
  OTTO_ASSERT_EQI(buf.capacity, capacity);

  log_debug("Checking that the push happened fine");
  uint32_t contained_val;
  OTTO_CALL(otto_buffer_get(&buf, 0, &contained_val), "Failed getting buf[0]");
  OTTO_ASSERT_EQI(contained_val, val);
  return OTTO_STATUS_SUCCESS;
}

otto_status_t test_modification_extend_array(void) {
  otto_buffer_t buf;
  uint32_t      data[] = {0, 1, 2, 3, 4, 5, 6, 7};
  size_t        len    = 8;

  log_debug("Creating buffer");
  OTTO_CALL(otto_buffer_from_array(data, len, sizeof(uint32_t), &buf), "Failed creating buffer");

  log_debug("Extending buffer");
  uint32_t values[]   = {20, 19, 15};
  size_t   values_len = 3;
  OTTO_CALL(otto_buffer_extend_array(&buf, &values, values_len), "Failed extending array");

  log_debug("Checking that the dimensions changed properly");
  OTTO_ASSERT_EQI(buf.len, 11);
  OTTO_ASSERT_EQI(buf.capacity, 11);

  log_debug("Checking border scenario");
  if (otto_buffer_extend_array(&buf, NULL, 10010).status != OTTO_FAILURE) {
    log_fatal("Extending with NULL did not fail");
    return OTTO_STATUS_FAILURE("Expected fail extending");
  }
  log_debug("Checking that the failed extend does not modify the buffer");
  OTTO_ASSERT_EQI(buf.len, 11);
  OTTO_ASSERT_EQI(buf.capacity, 11);

  log_debug("Checking that the extend happened fine");
  uint32_t contained_val;
  for (uint32_t i = 0; i < len; i++) {
    OTTO_CALL(otto_buffer_get(&buf, i, &contained_val), "Failed getting buf[%d]", i);
    OTTO_ASSERT_EQI(contained_val, data[i]);
  }
  for (uint32_t i = 0; i < values_len; i++) {
    OTTO_CALL(otto_buffer_get(&buf, i + len, &contained_val), "Failed getting buf[%d]", i + len);
    OTTO_ASSERT_EQI(contained_val, values[i]);
  }
  log_debug("Found no issues");
  return OTTO_STATUS_SUCCESS;
}

int main(void) {
  OTTO_CALL_TEST(test_modification_push1);
  OTTO_CALL_TEST(test_modification_push2);
  OTTO_CALL_TEST(test_modification_extend_array);

  return TEST_PASS;
}
