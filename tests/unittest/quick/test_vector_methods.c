#include <stddef.h>
#include <stdint.h>

#include <otto/status.h>
#include <otto/vector.h>
#include <otto_utils/test.h>
#include <ottou/log.h>
#include <ottou/macros.h>

otto_status_t test_get(void) {
  otto_vector_t vec;
  uint32_t data[] = {7, 6, 5, 4, 3, 2, 1, 0};
  size_t len = 8;

  log_debug("Creating vector");
  OTTO_CALL(otto_vector_from_array(data, len, sizeof(uint32_t), &vec),
            "Failed creating vector");

  log_debug("Checking all the elements");
  uint32_t val;
  for (uint32_t i = 0; i < len; i++) {
    OTTO_CALL(otto_vector_get(&vec, i, &val), "Failed getting vec[%d]", i);
    OTTO_ASSERT_EQI(val, data[i]);
  }

  log_debug("Checking border scenarios");
  if (otto_vector_get(&vec, len, &val).status != OTTO_FAILURE) {
    log_fatal("Getting element %d did not fail", len);
    return OTTO_STATUS_FAILURE("Expected fail getting element");
  }

  // Check that the failed call to otto_vector_get did not accidentally
  // modify val
  OTTO_ASSERT_EQI(val, data[len - 1]);
  return OTTO_STATUS_SUCCESS;
}

otto_status_t test_set(void) {
  otto_vector_t vec;
  uint32_t data[] = {7, 6, 5, 4, 3, 2, 1, 0};
  size_t len = 8;

  log_debug("Creating vector");
  OTTO_CALL(otto_vector_from_array(data, len, sizeof(uint32_t), &vec),
            "Failed creating vector");

  log_debug("Checking all the elements before setting");
  uint32_t val;
  for (uint32_t i = 0; i < len; i++) {
    OTTO_CALL(otto_vector_get(&vec, i, &val), "Failed getting vec[%d]", i);
    OTTO_ASSERT_EQI(val, data[i]);
  }

  log_debug("Changing two elements");
  size_t i0 = 2;
  size_t i1 = 6;
  uint32_t new0 = 20;
  uint32_t new1 = 1881;
  OTTO_CALL(otto_vector_set(&vec, i0, &new0), "Failed ");
  OTTO_CALL(otto_vector_set(&vec, i1, &new1), "Failed ");

  log_debug("Checking elements after setting");
  for (uint32_t i = 0; i < len; i++) {
    OTTO_CALL(otto_vector_get(&vec, i, &val), "Failed getting vec[%d]", i);
    if (i == i0) {
      OTTO_ASSERT_EQI(val, new0);
    } else if (i == i1) {
      OTTO_ASSERT_EQI(val, new1);
    } else {
      OTTO_ASSERT_EQI(val, data[i]);
    }
  }

  log_debug("Checking border scenarios");
  if (otto_vector_set(&vec, len - 1, NULL).status != OTTO_FAILURE) {
    log_fatal("Setting element %d did not fail", len - 1);
    return OTTO_STATUS_FAILURE("Expected fail setting element");
  }

  // Check that the failed call to otto_vector_get did not accidentally
  // modify val
  OTTO_ASSERT_EQI(val, data[len - 1]);
  return OTTO_STATUS_SUCCESS;
}

int main(void) {
  OTTO_CALL_TEST(test_get);
  OTTO_CALL_TEST(test_set);
  return TEST_PASS;
}
