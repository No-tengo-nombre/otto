#include <stddef.h>
#include <stdint.h>

#include <otto/status.h>
#include <otto/vector.h>

#include "src/otto_utils/test.h"

int test_get() {
  log_info("Testing `get`");
  otto_vector_t vec;
  uint32_t data[] = {7, 6, 5, 4, 3, 2, 1, 0};
  size_t len = 8;
  otto_status_t status = OTTO_STATUS_SUCCESS;

  log_debug("Creating vector");
  status &= otto_vector_from_array(data, len, sizeof(uint32_t), &vec);

  log_debug("Checking all the elements");
  uint32_t val;
  for (int i = 0; i < len; i++) {
    status &= otto_vector_get(&vec, i, &val);
    if (data[i] != val) {
      return TEST_FAIL;
    }
  }

  log_debug("Checking border scenarios");
  if (otto_vector_get(&vec, len, &val) != OTTO_STATUS_FAILURE) {
    return TEST_FAIL;
  }

  // Check that the failed call to otto_vector_get did not accidentally
  // modify val
  if (val != data[len - 1]) {
    return TEST_FAIL;
  }

  if (status == OTTO_STATUS_FAILURE) {
    return TEST_FAIL;
  }
  return TEST_PASS;
}

int main() {
  OTTO_CALL_TEST(test_get);
  return TEST_PASS;
}
