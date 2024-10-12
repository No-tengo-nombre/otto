#include <otto/tensor.h>
#include <otto_utils/test.h>
#include <ottou/log.h>
#include <ottou/macros.h>
#include <stddef.h>
#include <stdint.h>

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

otto_status_t test_methods_get_rank2() {
  double data[3][4] = {
      {  1.0,   2.0,   3.0,   4.0},
      { 10.0,  20.0,  30.0,  40.0},
      {-10.0, -20.0, -30.0, -40.0},
  };
  size_t rank      = 2;
  size_t shape[]   = {3, 4};
  size_t data_size = sizeof(double);

  logi_debug("Creating tensor");
  otto_tensor_t mat;
  OTTO_CALL(otto_tensor_from_array(data, rank, shape, data_size, &mat), "Failed creating tensor from array");

  double val;
  size_t idx[] = {1, 2};
  OTTO_CALL(otto_tensor_get(&mat, idx, &val), "Failed getting element");
  OTTO_ASSERT_EQF(val, 30.0);
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

otto_status_t test_methods_set_rank2() {
  double data[3][4] = {
      {  1.0,   2.0,   3.0,   4.0},
      { 10.0,  20.0,  30.0,  40.0},
      {-10.0, -20.0, -30.0, -40.0},
  };
  size_t rank      = 2;
  size_t shape[]   = {3, 4};
  size_t data_size = sizeof(double);

  logi_debug("Creating tensor");
  otto_tensor_t mat;
  OTTO_CALL(otto_tensor_from_array(data, rank, shape, data_size, &mat), "Failed creating tensor from array");

  double new_val = 1236.124;
  size_t idx[]   = {2, 2};
  OTTO_CALL(otto_tensor_set(&mat, idx, &new_val), "Failed setting element");
  OTTO_ASSERT_EQF(((double *)mat.buf.data)[10], new_val);
  return OTTO_STATUS_SUCCESS;
}

int main() {
  OTTO_CALL_TEST(test_methods_get1);
  OTTO_CALL_TEST(test_methods_get2);
  OTTO_CALL_TEST(test_methods_get3);
  OTTO_CALL_TEST(test_methods_get_rank2);
  OTTO_CALL_TEST(test_methods_set);
  OTTO_CALL_TEST(test_methods_set_rank2);
  return TEST_PASS;
}
