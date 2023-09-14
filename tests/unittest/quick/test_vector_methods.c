#include <stddef.h>
#include <stdint.h>

#include <otto/status.h>
#include <otto/vector.h>

#include "src/otto_utils/test.h"

int test_get() {
  otto_vector_t vec;
  uint32_t data[] = {7, 6, 5, 4, 3, 2, 1, 0};
  size_t len = 8;
  otto_status_t status = OTTO_STATUS_SUCCESS;

  status &= otto_vector_from_array(data, len, sizeof(uint32_t), &vec);

  uint32_t val;
  for (int i = 0; i < len; i++) {
    status &= otto_vector_get(&vec, i, &val);
    if (data[i] != val) {
      return TEST_FAIL;
    }
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
