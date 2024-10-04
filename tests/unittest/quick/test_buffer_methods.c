#include <otto/buffer.h>
#include <otto/devices.h>
#include <otto/status.h>
#include <otto_utils/test.h>
#include <ottou/log.h>
#include <ottou/macros.h>
#include <stddef.h>
#include <stdint.h>

otto_status_t test_methods_get(void) {
  otto_buffer_t buf;
  uint32_t      data[] = {7, 6, 5, 4, 3, 2, 1, 0};
  size_t        len    = 8;

  log_debug("Creating buffer");
  OTTO_CALL(otto_buffer_from_array(data, len, sizeof(uint32_t), &buf), "Failed creating buffer");

  log_debug("Checking all the elements");
  uint32_t val;
  for (uint32_t i = 0; i < len; i++) {
    OTTO_CALL(otto_buffer_get(&buf, i, &val), "Failed getting buf[%d]", i);
    OTTO_ASSERT_EQI(val, data[i]);
  }

  log_debug("Checking border scenarios");
  if (otto_buffer_get(&buf, len, &val).status != OTTO_FAILURE) {
    log_fatal("Getting element %d did not fail", len);
    return OTTO_STATUS_FAILURE("Expected fail getting element");
  }

  // Check that the failed call to otto_buffer_get did not accidentally
  // modify val
  OTTO_ASSERT_EQI(val, data[len - 1]);
  return OTTO_STATUS_SUCCESS;
}

otto_status_t test_methods_set(void) {
  otto_buffer_t buf;
  uint32_t      data[] = {7, 6, 5, 4, 3, 2, 1, 0};
  size_t        len    = 8;

  log_debug("Creating buffer");
  OTTO_CALL(otto_buffer_from_array(data, len, sizeof(uint32_t), &buf), "Failed creating buffer");

  log_debug("Checking all the elements before setting");
  uint32_t val;
  for (uint32_t i = 0; i < len; i++) {
    OTTO_CALL(otto_buffer_get(&buf, i, &val), "Failed getting buf[%d]", i);
    OTTO_ASSERT_EQI(val, data[i]);
  }

  log_debug("Changing two elements");
  size_t   i0   = 2;
  size_t   i1   = 6;
  uint32_t new0 = 20;
  uint32_t new1 = 1881;
  OTTO_CALL(otto_buffer_set(&buf, i0, &new0), "Failed ");
  OTTO_CALL(otto_buffer_set(&buf, i1, &new1), "Failed ");

  log_debug("Checking elements after setting");
  for (uint32_t i = 0; i < len; i++) {
    OTTO_CALL(otto_buffer_get(&buf, i, &val), "Failed getting buf[%d]", i);
    if (i == i0) {
      OTTO_ASSERT_EQI(val, new0);
    } else if (i == i1) {
      OTTO_ASSERT_EQI(val, new1);
    } else {
      OTTO_ASSERT_EQI(val, data[i]);
    }
  }

  log_debug("Checking border scenarios");
  if (otto_buffer_set(&buf, len - 1, NULL).status != OTTO_FAILURE) {
    log_fatal("Setting element %d did not fail", len - 1);
    return OTTO_STATUS_FAILURE("Expected fail setting element");
  }

  // Check that the failed call to otto_buffer_get did not accidentally
  // modify val
  OTTO_ASSERT_EQI(val, data[len - 1]);
  return OTTO_STATUS_SUCCESS;
}

int main(void) {
  OTTO_CALL_TEST(test_methods_get);
  OTTO_CALL_TEST(test_methods_set);

  return TEST_PASS;
}
