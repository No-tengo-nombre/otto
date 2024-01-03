#include <stddef.h>
#include <stdint.h>

#include <otto/tensor.h>
#include <otto_utils/test.h>
#include <ottou/log.h>
#include <ottou/macros.h>

otto_status_t test_creation_zero() {
  otto_tensor_t mat;
  size_t shape[] = {5, 3, 3};
  OTTO_CALL(otto_tensor_zero(3, shape, sizeof(uint32_t), &mat),
            "Failed creating tensor");
  size_t len = 45;
  OTTO_ASSERT_EQLI(mat.rank, 3);
  OTTO_ASSERT_EQLI(mat.shape[0], 5);
  OTTO_ASSERT_EQLI(mat.shape[1], 3);
  OTTO_ASSERT_EQLI(mat.shape[2], 3);
  for (int i = 0; i < len; i++) {
    OTTO_ASSERT_EQI(((uint32_t *)mat.vec.data)[i], 0);
  }
  return OTTO_STATUS_SUCCESS;
}

otto_status_t test_creation_zero2() {
  otto_tensor_t mat;
  size_t shape[] = {20};
  OTTO_CALL(otto_tensor_zero(1, shape, sizeof(uint32_t), &mat),
            "Failed creating tensor");
  size_t len = 20;
  OTTO_ASSERT_EQLI(mat.rank, 1);
  OTTO_ASSERT_EQLI(mat.shape[0], 20);
  for (int i = 0; i < len; i++) {
    OTTO_ASSERT_EQI(((uint32_t *)mat.vec.data)[i], 0);
  }
  return OTTO_STATUS_SUCCESS;
}

int main() {
  OTTO_CALL_TEST(test_creation_zero);
  OTTO_CALL_TEST(test_creation_zero2);
  return TEST_PASS;
}
