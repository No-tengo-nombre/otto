#include <otto/tensor.h>
#include <otto_utils/test.h>
#include <ottou/log.h>
#include <ottou/macros.h>
#include <stddef.h>
#include <stdint.h>

otto_status_t test_creation_zero1() {
  otto_tensor_t mat;
  size_t        shape[] = {5, 3, 4};
  OTTO_CALL(otto_tensor_zero(3, shape, sizeof(uint32_t), &mat), "Failed creating tensor");
  size_t len = 60;
  OTTO_ASSERT_EQI(mat.rank, 3);
  OTTO_ASSERT_EQI(mat.shape[0], 5);
  OTTO_ASSERT_EQI(mat.shape[1], 3);
  OTTO_ASSERT_EQI(mat.shape[2], 4);
  for (int i = 0; i < len; i++) {
    OTTO_ASSERT_EQI(((uint32_t *)mat.buf.data)[i], 0);
  }
  return OTTO_STATUS_SUCCESS;
}

otto_status_t test_creation_zero2() {
  otto_tensor_t mat;
  size_t        shape[] = {20};
  OTTO_CALL(otto_tensor_zero(1, shape, sizeof(uint32_t), &mat), "Failed creating tensor");
  size_t len = 20;
  OTTO_ASSERT_EQI(mat.rank, 1);
  OTTO_ASSERT_EQI(mat.shape[0], 20);
  for (int i = 0; i < len; i++) {
    OTTO_ASSERT_EQI(((uint32_t *)mat.buf.data)[i], 0);
  }
  return OTTO_STATUS_SUCCESS;
}

otto_status_t test_methods_get1() {
  otto_tensor_t mat;
  size_t        shape[] = {5, 3, 4};
  otto_tensor_zero(3, shape, sizeof(int32_t), &mat);
  int32_t val     = 123535;
  size_t  index[] = {0, 1, 0};
  OTTO_CALL(otto_tensor_get(&mat, index, &val), "Failed getting element");
  OTTO_ASSERT_EQI(val, 0);
  return OTTO_STATUS_SUCCESS;
}

otto_status_t test_methods_get2() {
  otto_tensor_t mat;
  size_t        shape[] = {5, 3, 4};
  otto_tensor_zero(3, shape, sizeof(int32_t), &mat);
  int32_t val     = 123535;
  size_t  index[] = {4, 2, 3};
  OTTO_CALL(otto_tensor_get(&mat, index, &val), "Failed getting element");
  OTTO_ASSERT_EQI(val, 0);
  return OTTO_STATUS_SUCCESS;
}

otto_status_t test_methods_get3() {
  otto_tensor_t mat;
  size_t        shape[] = {5, 3, 4};
  otto_tensor_zero(3, shape, sizeof(int32_t), &mat);
  int32_t val     = 123535;
  size_t  index[] = {5, 0, 0};
  OTTO_ASSERT_FAILS(otto_tensor_get(&mat, index, &val));
  return OTTO_STATUS_SUCCESS;
}

otto_status_t test_methods_set() {
  otto_tensor_t mat;
  size_t        shape[] = {5, 3, 4};
  otto_tensor_zero(3, shape, sizeof(int32_t), &mat);
  int32_t val     = 123535;
  size_t  index[] = {4, 2, 3};
  OTTO_CALL(otto_tensor_set(&mat, index, &val), "Failed setting element");

  int32_t test_val      = 1235;
  size_t  other_index[] = {4, 2, 2};
  OTTO_CALL(otto_tensor_get(&mat, index, &test_val), "Failed getting element");
  OTTO_ASSERT_EQI(test_val, 123535);
  OTTO_CALL(otto_tensor_get(&mat, other_index, &test_val), "Failed getting other element");
  OTTO_ASSERT_EQI(test_val, 0);
  return OTTO_STATUS_SUCCESS;
}

int main() {
  OTTO_CALL_TEST(test_creation_zero1);
  OTTO_CALL_TEST(test_creation_zero2);
  OTTO_CALL_TEST(test_methods_get1);
  OTTO_CALL_TEST(test_methods_get2);
  OTTO_CALL_TEST(test_methods_get3);
  OTTO_CALL_TEST(test_methods_set);
  return TEST_PASS;
}
