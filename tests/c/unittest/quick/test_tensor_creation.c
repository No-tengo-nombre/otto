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

otto_status_t test_creation_array_rank2() {
  logi_debug("Initializing values");
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

  logi_debug("Testing metadata");
  OTTO_ASSERT_EQI(mat.rank, 2);
  OTTO_ASSERT_EQI(mat.shape[0], 3);
  OTTO_ASSERT_EQI(mat.shape[1], 4);
  OTTO_ASSERT_EQI(mat.buf.data_size, sizeof(double));
  OTTO_ASSERT_EQI(mat.buf.len, shape[0] * shape[1]);
  OTTO_ASSERT_EQI(mat.buf.capacity, shape[0] * shape[1]);

  logi_debug("Testing contained values");
  double *values = (double *)mat.buf.data;
  OTTO_ASSERT_EQF(values[0], 1.0);
  OTTO_ASSERT_EQF(values[1], 2.0);
  OTTO_ASSERT_EQF(values[2], 3.0);
  OTTO_ASSERT_EQF(values[3], 4.0);
  OTTO_ASSERT_EQF(values[4], 10.0);
  OTTO_ASSERT_EQF(values[5], 20.0);
  OTTO_ASSERT_EQF(values[6], 30.0);
  OTTO_ASSERT_EQF(values[7], 40.0);
  OTTO_ASSERT_EQF(values[8], -10.0);
  OTTO_ASSERT_EQF(values[9], -20.0);
  OTTO_ASSERT_EQF(values[10], -30.0);
  OTTO_ASSERT_EQF(values[11], -40.0);

  return OTTO_STATUS_SUCCESS;
}

int main() {
  OTTO_CALL_TEST(test_creation_zero1);
  OTTO_CALL_TEST(test_creation_zero2);
  OTTO_CALL_TEST(test_creation_array_rank2);
  return TEST_PASS;
}
