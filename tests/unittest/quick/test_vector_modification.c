#include <stddef.h>
#include <stdint.h>

#include <otto/status.h>
#include <otto/vector.h>
#include <otto_utils/test.h>
#include <ottou/log.h>
#include <ottou/macros.h>

otto_status_t test_push(void) {
  otto_vector_t vec;
  uint32_t data[] = {0, 1, 2, 3, 4, 5, 6, 7};
  size_t len = 8;

  log_debug("Creating vector");
  OTTO_CALL(otto_vector_from_array(data, len, sizeof(uint32_t), &vec),
            "Failed creating vector");

  log_debug("Puhsing element");
  uint32_t val = 20;
  OTTO_CALL(otto_vector_push(&vec, &val), "Failed pushing element");

  log_debug("Checking that the dimensions changed properly");
  OTTO_ASSERT_EQI(vec.len, 9);
  OTTO_ASSERT_EQI(vec.capacity, 9);

  log_debug("Checking border scenario");
  if (otto_vector_push(&vec, NULL).status != OTTO_FAILURE) {
    log_fatal("Pushing NULL did not fail");
    return OTTO_STATUS_FAILURE("Expected fail pushing element");
  }
  log_debug("Checking that the failed push does not modify the vector");
  OTTO_ASSERT_EQI(vec.len, 9);
  OTTO_ASSERT_EQI(vec.capacity, 9);

  log_debug("Checking that the push happened fine");
  uint32_t contained_val;
  OTTO_CALL(otto_vector_get(&vec, 8, &contained_val), "Failed getting vec[8]");
  OTTO_ASSERT_EQI(contained_val, val);
  return OTTO_STATUS_SUCCESS;
}

otto_status_t test_push2(void) {
  otto_vector_t vec;
  size_t capacity = 8;

  log_debug("Creating vector");
  OTTO_CALL(otto_vector_with_capacity(capacity, sizeof(uint32_t), &vec),
            "Failed creating vector");

  log_debug("Pushing element");
  uint32_t val = 20;
  OTTO_CALL(otto_vector_push(&vec, &val), "Failed pushing element");

  log_debug("Checking that the dimensions changed properly");
  OTTO_ASSERT_EQI(vec.len, 1);
  OTTO_ASSERT_EQI(vec.capacity, capacity);

  log_debug("Checking that the push happened fine");
  uint32_t contained_val;
  OTTO_CALL(otto_vector_get(&vec, 0, &contained_val), "Failed getting vec[0]");
  OTTO_ASSERT_EQI(contained_val, val);
  return OTTO_STATUS_SUCCESS;
}

otto_status_t test_extend_array(void) {
  otto_vector_t vec;
  uint32_t data[] = {0, 1, 2, 3, 4, 5, 6, 7};
  size_t len = 8;

  log_debug("Creating vector");
  OTTO_CALL(otto_vector_from_array(data, len, sizeof(uint32_t), &vec),
            "Failed creating vector");

  log_debug("Extending vector");
  uint32_t values[] = {20, 19, 15};
  size_t values_len = 3;
  OTTO_CALL(otto_vector_extend_array(&vec, &values, values_len),
            "Failed extending array");

  log_debug("Checking that the dimensions changed properly");
  OTTO_ASSERT_EQI(vec.len, 11);
  OTTO_ASSERT_EQI(vec.capacity, 11);

  log_debug("Checking border scenario");
  if (otto_vector_extend_array(&vec, NULL, 10010).status != OTTO_FAILURE) {
    log_fatal("Extending with NULL did not fail");
    return OTTO_STATUS_FAILURE("Expected fail extending");
  }
  log_debug("Checking that the failed extend does not modify the vector");
  OTTO_ASSERT_EQI(vec.len, 11);
  OTTO_ASSERT_EQI(vec.capacity, 11);

  log_debug("Checking that the extend happened fine");
  uint32_t contained_val;
  for (uint32_t i = 0; i < len; i++) {
    OTTO_CALL(otto_vector_get(&vec, i, &contained_val),
              "Failed getting vec[%d]", i);
    OTTO_ASSERT_EQI(contained_val, data[i]);
  }
  for (uint32_t i = 0; i < values_len; i++) {
    OTTO_CALL(otto_vector_get(&vec, i + len, &contained_val),
              "Failed getting vec[%d]", i + len);
    OTTO_ASSERT_EQI(contained_val, values[i]);
  }
  log_debug("Found no issues");
  return OTTO_STATUS_SUCCESS;
}

int main(void) {
  OTTO_CALL_TEST(test_push);
  OTTO_CALL_TEST(test_push2);
  OTTO_CALL_TEST(test_extend_array);
  return TEST_PASS;
}
