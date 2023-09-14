#include <stddef.h>
#include <stdint.h>

#include <otto/status.h>
#include <otto/vector.h>

#include "ottou/log.h"
#include "src/otto_utils/test.h"

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
  status &= otto_vector_push(&val, &vec);

  log_debug("Checking that the dimensions changed properly");
  if (vec.size != 9 || vec.capacity != 9) {
    return TEST_FAIL;
  }

  log_debug("Checking border scenario");
  if (otto_vector_push(NULL, &vec) != OTTO_STATUS_FAILURE) {
    return TEST_FAIL;
  }
  log_debug("Checking that the failed push does not modify the vector");
  if (vec.size != 9 || vec.capacity != 9) {
    return TEST_FAIL;
  }

  log_debug("Checking that the push happened fine");
  uint32_t contained_val;
  status &= otto_vector_get(&vec, 8, &contained_val);
  if (contained_val != val) {
    return TEST_FAIL;
  }

  if (status == OTTO_STATUS_FAILURE) {
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
  status &= otto_vector_extend_array(&values, values_len, &vec);

  log_debug("Checking that the dimensions changed properly");
  if (vec.size != 11 || vec.capacity != 11) {
    return TEST_FAIL;
  }

  log_debug("Checking border scenario");
  if (otto_vector_extend_array(NULL, 10010, &vec) != OTTO_STATUS_FAILURE) {
    return TEST_FAIL;
  }
  log_debug("Checking that the failed extend does not modify the vector");
  if (vec.size != 11 || vec.capacity != 11) {
    return TEST_FAIL;
  }

  log_debug("Checking that the extend happened fine");
  uint32_t contained_val;
  for (int i = 0; i < values_len; i++) {
    status &= otto_vector_get(&vec, i + len, &contained_val);
    if (contained_val != values[i]) {
      return TEST_FAIL;
    }
  }

  if (status == OTTO_STATUS_FAILURE) {
    return TEST_FAIL;
  }
  return TEST_PASS;
}

int main() {
  OTTO_CALL_TEST(test_push);
  OTTO_CALL_TEST(test_extend_array);
  return TEST_PASS;
}
