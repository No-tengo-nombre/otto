#include <stddef.h>
#include <stdint.h>

#include <otto/devices.h>
#include <otto/status.h>
#include <otto/vector.h>
#include <otto_utils/test.h>
#include <ottou/log.h>
#include <ottou/macros.h>

otto_status_t test_creation_new(void) {
  otto_vector_t vec;

  log_debug("Creating vector");
  OTTO_CALL(otto_vector_new(sizeof(uint32_t), &vec), "Failed creating vector");

  log_debug("Checking creation result");
  OTTO_ASSERT_EQI(vec.data, NULL);
  OTTO_ASSERT_EQI(vec.capacity, 0);
  OTTO_ASSERT_EQI(vec.device, OTTO_DEVICE_CPU);
  OTTO_ASSERT_EQI(vec.len, 0);
  return OTTO_STATUS_SUCCESS;
}

otto_status_t test_creation_zero(void) {
  otto_vector_t vec;
  size_t len = 10;

  log_info("Creating vector");
  OTTO_CALL(otto_vector_zero(len, sizeof(uint32_t), &vec),
            "Failed creating vector");

  log_debug("Checking creation result");
  for (uint32_t i = 0; i < len; i++) {
    OTTO_ASSERT_EQI(*(uint32_t *)((char *)vec.data + i * sizeof(uint32_t)), 0);
  }
  OTTO_ASSERT_EQI(vec.device, OTTO_DEVICE_CPU);
  return OTTO_STATUS_SUCCESS;
}

otto_status_t test_creation_with_capacity(void) {
  otto_vector_t vec;

  log_debug("Creating vector");
  OTTO_CALL(otto_vector_with_capacity(6, sizeof(uint32_t), &vec),
            "Failed creating vector");

  log_debug("Checking creation result");
  OTTO_ASSERT_NEI(vec.data, NULL);
  OTTO_ASSERT_NEI(vec.capacity, 0);
  OTTO_ASSERT_EQI(vec.data_size, sizeof(uint32_t));
  OTTO_ASSERT_EQI(vec.device, OTTO_DEVICE_CPU);
  OTTO_ASSERT_EQI(vec.len, 0);
  return OTTO_STATUS_SUCCESS;
}

otto_status_t test_creation_from_array(void) {
  otto_vector_t vec;
  uint32_t data[] = {0, 1, 2, 3, 4, 5, 6, 7};
  size_t len = 8;

  log_info("Creating vector");
  OTTO_CALL(otto_vector_from_array(data, len, sizeof(uint32_t), &vec),
            "Failed creating vector");

  log_debug("Checking creation result");
  for (uint32_t i = 0; i < len; i++) {
    OTTO_ASSERT_EQI(*(uint32_t *)((char *)vec.data + i * sizeof(uint32_t)),
                    data[i]);
  }
  OTTO_ASSERT_EQI(vec.device, OTTO_DEVICE_CPU);

  return OTTO_STATUS_SUCCESS;
}

otto_status_t test_methods_get(void) {
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

otto_status_t test_methods_set(void) {
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

otto_status_t test_modification_push(void) {
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

otto_status_t test_modification_push2(void) {
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

otto_status_t test_modification_extend_array(void) {
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
  OTTO_CALL_TEST(test_creation_new);
  OTTO_CALL_TEST(test_creation_zero);
  OTTO_CALL_TEST(test_creation_with_capacity);
  OTTO_CALL_TEST(test_creation_from_array);

  OTTO_CALL_TEST(test_methods_get);
  OTTO_CALL_TEST(test_methods_set);

  OTTO_CALL_TEST(test_modification_push);
  OTTO_CALL_TEST(test_modification_push2);
  OTTO_CALL_TEST(test_modification_extend_array);

  return TEST_PASS;
}
