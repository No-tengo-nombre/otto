#include <stddef.h>
#include <stdint.h>

#include <otto/status.h>
#include <otto/vector.h>
#include <ottou/log.h>
#include <src/otto_utils/test.h>

int test_push() {
  log_info("Testing `push`");
  otto_vector_t vec;
  uint32_t data[] = {0, 1, 2, 3, 4, 5, 6, 7};
  size_t len = 8;
  otto_status_t status = OTTO_STATUS_SUCCESS;

  log_debug("Creating vector");
  status &= otto_vector_from_array(data, len, sizeof(uint32_t), &vec);

  log_debug("Puhsing element");
  uint32_t val = 20;
  status &= otto_vector_push(&vec, &val);

  log_debug("Checking that the dimensions changed properly");
  if (vec.len != 9 || vec.capacity != 9) {
    log_fatal("[FAIL] Found incorrect length or capacity");
    return TEST_FAIL;
  }

  log_debug("Checking border scenario");
  if (otto_vector_push(&vec, NULL) != OTTO_STATUS_FAILURE) {
    log_fatal("[FAIL] Pushing NULL did not fail");
    return TEST_FAIL;
  }
  log_debug("Checking that the failed push does not modify the vector");
  if (vec.len != 9 || vec.capacity != 9) {
    log_fatal("[FAIL] Failed operation modified the vector");
    return TEST_FAIL;
  }

  log_debug("Checking that the push happened fine");
  uint32_t contained_val;
  status &= otto_vector_get(&vec, 8, &contained_val);
  if (contained_val != val) {
    log_fatal("[FAIL] %d != %d", contained_val, val);
    return TEST_FAIL;
  }

  if (status == OTTO_STATUS_FAILURE) {
    log_fatal("[FAIL] Some operation failed");
    return TEST_FAIL;
  }
  return TEST_PASS;
}

int test_push2() {
  log_info("Testing `push2`");
  otto_vector_t vec;
  otto_status_t status = OTTO_STATUS_SUCCESS;
  size_t capacity = 8;

  log_debug("Creating vector");
  status &= otto_vector_with_capacity(capacity, sizeof(uint32_t), &vec);

  log_debug("Pushing element");
  uint32_t val = 20;
  status &= otto_vector_push(&vec, &val);

  log_debug("Checking that the dimensions changed properly");
  if (vec.len != 1 || vec.capacity != 8) {
    log_fatal("[FAIL] Found incorrect length or capacity");
    return TEST_FAIL;
  }

  log_debug("Checking that the push happened fine");
  uint32_t contained_val;
  status &= otto_vector_get(&vec, 0, &contained_val);
  if (contained_val != val) {
    log_fatal("[FAIL] %d != %d", contained_val, val);
    return TEST_FAIL;
  }

  if (status == OTTO_STATUS_FAILURE) {
    log_fatal("[FAIL] Some operation failed");
    return TEST_FAIL;
  }
  return TEST_PASS;
}

int test_extend_array() {
  log_info("Testing `extend_array`");
  otto_vector_t vec;
  uint32_t data[] = {0, 1, 2, 3, 4, 5, 6, 7};
  size_t len = 8;
  otto_status_t status = OTTO_STATUS_SUCCESS;

  log_debug("Creating vector");
  status &= otto_vector_from_array(data, len, sizeof(uint32_t), &vec);

  log_debug("Extending vector");
  uint32_t values[] = {20, 19, 15};
  size_t values_len = 3;
  status &= otto_vector_extend_array(&vec, &values, values_len);

  log_debug("Checking that the dimensions changed properly");
  if (vec.len != 11 || vec.capacity != 11) {
    log_fatal("[FAIL] Found incorrect length or capacity");
    return TEST_FAIL;
  }

  log_debug("Checking border scenario");
  if (otto_vector_extend_array(&vec, NULL, 10010) != OTTO_STATUS_FAILURE) {
    log_fatal("[FAIL] Extending with NULL did not fail");
    return TEST_FAIL;
  }
  log_debug("Checking that the failed extend does not modify the vector");
  if (vec.len != 11 || vec.capacity != 11) {
    log_fatal("[FAIL] Failed operation modified the vector");
    return TEST_FAIL;
  }

  log_debug("Checking that the extend happened fine");
  uint32_t contained_val;
  for (int i = 0; i < values_len; i++) {
    status &= otto_vector_get(&vec, i + len, &contained_val);
    if (contained_val != values[i]) {
      log_fatal("[FAIL] %d != %d", contained_val, values[i]);
      return TEST_FAIL;
    }
  }

  if (status == OTTO_STATUS_FAILURE) {
    log_fatal("[FAIL] Some operation failed");
    return TEST_FAIL;
  }
  return TEST_PASS;
}

int main() {
  OTTO_CALL_TEST(test_push);
  OTTO_CALL_TEST(test_push2);
  OTTO_CALL_TEST(test_extend_array);
  return TEST_PASS;
}
