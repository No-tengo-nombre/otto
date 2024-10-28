#include <otto/buffer.h>
#include <otto/devices.h>
#include <otto/status.h>
#include <otto_utils/test.h>
#include <ottou/log.h>
#include <ottou/macros.h>
#include <stddef.h>
#include <stdint.h>

otto_status_t test_methods_get(void) {
  otto_buffer_t buf;
  uint32_t      data[] = {7, 6, 5, 4, 3, 2, 1, 0};
  size_t        len    = 8;

  log_debug("Creating buffer");
  OTTO_CALL(otto_buffer_from_array(data, len, sizeof(uint32_t), &buf), "Failed creating buffer");

  log_debug("Checking all the elements");
  uint32_t val;
  for (uint32_t i = 0; i < len; i++) {
    OTTO_CALL(otto_buffer_get(&buf, i, &val), "Failed getting buf[%d]", i);
    OTTO_ASSERT_EQI(val, data[i]);
  }

  log_debug("Checking border scenarios");
  if (otto_buffer_get(&buf, len, &val).status != OTTO_FAILURE) {
    log_fatal("Getting element %d did not fail", len);
    return OTTO_STATUS_FAILURE("Expected fail getting element");
  }

  // Check that the failed call to otto_buffer_get did not accidentally
  // modify val
  OTTO_ASSERT_EQI(val, data[len - 1]);
  return OTTO_STATUS_SUCCESS;
}

otto_status_t test_methods_get_i8(void) {
  otto_buffer_t buf;
  int8_t        data[] = {7, 6, 5, 4, 3, 2, 1, 0};
  size_t        len    = 8;

  log_debug("Creating buffer");
  OTTO_CALL(otto_buffer_from_array(data, len, sizeof(int8_t), &buf), "Failed creating buffer");

  log_debug("Checking all the elements");
  int8_t val;
  for (uint32_t i = 0; i < len; i++) {
    val = otto_buffer_get_i8(&buf, i);
    OTTO_ASSERT_EQI(val, data[i]);
  }

  log_debug("Checking border scenarios");
  if (otto_buffer_get(&buf, len, &val).status != OTTO_FAILURE) {
    log_fatal("Getting element %d did not fail", len);
    return OTTO_STATUS_FAILURE("Expected fail getting element");
  }

  OTTO_ASSERT_EQI(val, data[len - 1]);
  return OTTO_STATUS_SUCCESS;
}

otto_status_t test_methods_get_i16(void) {
  otto_buffer_t buf;
  int16_t       data[] = {7, 6, 5, 4, 3, 2, 1, 0};
  size_t        len    = 8;

  log_debug("Creating buffer");
  OTTO_CALL(otto_buffer_from_array(data, len, sizeof(int16_t), &buf), "Failed creating buffer");

  log_debug("Checking all the elements");
  int16_t val;
  for (uint32_t i = 0; i < len; i++) {
    val = otto_buffer_get_i16(&buf, i);
    OTTO_ASSERT_EQI(val, data[i]);
  }

  log_debug("Checking border scenarios");
  if (otto_buffer_get(&buf, len, &val).status != OTTO_FAILURE) {
    log_fatal("Getting element %d did not fail", len);
    return OTTO_STATUS_FAILURE("Expected fail getting element");
  }

  OTTO_ASSERT_EQI(val, data[len - 1]);
  return OTTO_STATUS_SUCCESS;
}

otto_status_t test_methods_get_i32(void) {
  otto_buffer_t buf;
  int32_t       data[] = {7, 6, 5, 4, 3, 2, 1, 0};
  size_t        len    = 8;

  log_debug("Creating buffer");
  OTTO_CALL(otto_buffer_from_array(data, len, sizeof(int32_t), &buf), "Failed creating buffer");

  log_debug("Checking all the elements");
  int32_t val;
  for (uint32_t i = 0; i < len; i++) {
    val = otto_buffer_get_i32(&buf, i);
    OTTO_ASSERT_EQI(val, data[i]);
  }

  log_debug("Checking border scenarios");
  if (otto_buffer_get(&buf, len, &val).status != OTTO_FAILURE) {
    log_fatal("Getting element %d did not fail", len);
    return OTTO_STATUS_FAILURE("Expected fail getting element");
  }

  OTTO_ASSERT_EQI(val, data[len - 1]);
  return OTTO_STATUS_SUCCESS;
}

otto_status_t test_methods_get_i64(void) {
  otto_buffer_t buf;
  int64_t       data[] = {7, 6, 5, 4, 3, 2, 1, 0};
  size_t        len    = 8;

  log_debug("Creating buffer");
  OTTO_CALL(otto_buffer_from_array(data, len, sizeof(int64_t), &buf), "Failed creating buffer");

  log_debug("Checking all the elements");
  int64_t val;
  for (uint32_t i = 0; i < len; i++) {
    val = otto_buffer_get_i64(&buf, i);
    OTTO_ASSERT_EQI(val, data[i]);
  }

  log_debug("Checking border scenarios");
  if (otto_buffer_get(&buf, len, &val).status != OTTO_FAILURE) {
    log_fatal("Getting element %d did not fail", len);
    return OTTO_STATUS_FAILURE("Expected fail getting element");
  }

  OTTO_ASSERT_EQI(val, data[len - 1]);
  return OTTO_STATUS_SUCCESS;
}

otto_status_t test_methods_get_u8(void) {
  otto_buffer_t buf;
  uint8_t       data[] = {7, 6, 5, 4, 3, 2, 1, 0};
  size_t        len    = 8;

  log_debug("Creating buffer");
  OTTO_CALL(otto_buffer_from_array(data, len, sizeof(uint8_t), &buf), "Failed creating buffer");

  log_debug("Checking all the elements");
  uint8_t val;
  for (uint32_t i = 0; i < len; i++) {
    val = otto_buffer_get_u8(&buf, i);
    OTTO_ASSERT_EQI(val, data[i]);
  }

  log_debug("Checking border scenarios");
  if (otto_buffer_get(&buf, len, &val).status != OTTO_FAILURE) {
    log_fatal("Getting element %d did not fail", len);
    return OTTO_STATUS_FAILURE("Expected fail getting element");
  }

  OTTO_ASSERT_EQI(val, data[len - 1]);
  return OTTO_STATUS_SUCCESS;
}

otto_status_t test_methods_get_u16(void) {
  otto_buffer_t buf;
  uint16_t      data[] = {7, 6, 5, 4, 3, 2, 1, 0};
  size_t        len    = 8;

  log_debug("Creating buffer");
  OTTO_CALL(otto_buffer_from_array(data, len, sizeof(uint16_t), &buf), "Failed creating buffer");

  log_debug("Checking all the elements");
  int16_t val;
  for (uint32_t i = 0; i < len; i++) {
    val = otto_buffer_get_i16(&buf, i);
    OTTO_ASSERT_EQI(val, data[i]);
  }

  log_debug("Checking border scenarios");
  if (otto_buffer_get(&buf, len, &val).status != OTTO_FAILURE) {
    log_fatal("Getting element %d did not fail", len);
    return OTTO_STATUS_FAILURE("Expected fail getting element");
  }

  OTTO_ASSERT_EQI(val, data[len - 1]);
  return OTTO_STATUS_SUCCESS;
}

otto_status_t test_methods_get_u32(void) {
  otto_buffer_t buf;
  uint32_t      data[] = {7, 6, 5, 4, 3, 2, 1, 0};
  size_t        len    = 8;

  log_debug("Creating buffer");
  OTTO_CALL(otto_buffer_from_array(data, len, sizeof(uint32_t), &buf), "Failed creating buffer");

  log_debug("Checking all the elements");
  uint32_t val;
  for (uint32_t i = 0; i < len; i++) {
    val = otto_buffer_get_u32(&buf, i);
    OTTO_ASSERT_EQI(val, data[i]);
  }

  log_debug("Checking border scenarios");
  if (otto_buffer_get(&buf, len, &val).status != OTTO_FAILURE) {
    log_fatal("Getting element %d did not fail", len);
    return OTTO_STATUS_FAILURE("Expected fail getting element");
  }

  OTTO_ASSERT_EQI(val, data[len - 1]);
  return OTTO_STATUS_SUCCESS;
}

otto_status_t test_methods_get_u64(void) {
  otto_buffer_t buf;
  uint64_t      data[] = {7, 6, 5, 4, 3, 2, 1, 0};
  size_t        len    = 8;

  log_debug("Creating buffer");
  OTTO_CALL(otto_buffer_from_array(data, len, sizeof(uint64_t), &buf), "Failed creating buffer");

  log_debug("Checking all the elements");
  uint64_t val;
  for (uint32_t i = 0; i < len; i++) {
    val = otto_buffer_get_u64(&buf, i);
    OTTO_ASSERT_EQI(val, data[i]);
  }

  log_debug("Checking border scenarios");
  if (otto_buffer_get(&buf, len, &val).status != OTTO_FAILURE) {
    log_fatal("Getting element %d did not fail", len);
    return OTTO_STATUS_FAILURE("Expected fail getting element");
  }

  OTTO_ASSERT_EQI(val, data[len - 1]);
  return OTTO_STATUS_SUCCESS;
}

otto_status_t test_methods_get_f(void) {
  otto_buffer_t buf;
  float         data[] = {7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0, 0.0};
  size_t        len    = 8;

  log_debug("Creating buffer");
  OTTO_CALL(otto_buffer_from_array(data, len, sizeof(float), &buf), "Failed creating buffer");

  log_debug("Checking all the elements");
  float val;
  for (uint32_t i = 0; i < len; i++) {
    val = otto_buffer_get_f(&buf, i);
    OTTO_ASSERT_EQI(val, data[i]);
  }

  log_debug("Checking border scenarios");
  if (otto_buffer_get(&buf, len, &val).status != OTTO_FAILURE) {
    log_fatal("Getting element %d did not fail", len);
    return OTTO_STATUS_FAILURE("Expected fail getting element");
  }

  OTTO_ASSERT_EQI(val, data[len - 1]);
  return OTTO_STATUS_SUCCESS;
}

otto_status_t test_methods_get_d(void) {
  otto_buffer_t buf;
  double        data[] = {7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0, 0.0};
  size_t        len    = 8;

  log_debug("Creating buffer");
  OTTO_CALL(otto_buffer_from_array(data, len, sizeof(double), &buf), "Failed creating buffer");

  log_debug("Checking all the elements");
  double val;
  for (uint32_t i = 0; i < len; i++) {
    val = otto_buffer_get_d(&buf, i);
    OTTO_ASSERT_EQI(val, data[i]);
  }

  log_debug("Checking border scenarios");
  if (otto_buffer_get(&buf, len, &val).status != OTTO_FAILURE) {
    log_fatal("Getting element %d did not fail", len);
    return OTTO_STATUS_FAILURE("Expected fail getting element");
  }

  OTTO_ASSERT_EQI(val, data[len - 1]);
  return OTTO_STATUS_SUCCESS;
}

otto_status_t test_methods_set(void) {
  otto_buffer_t buf;
  uint32_t      data[] = {7, 6, 5, 4, 3, 2, 1, 0};
  size_t        len    = 8;

  log_debug("Creating buffer");
  OTTO_CALL(otto_buffer_from_array(data, len, sizeof(uint32_t), &buf), "Failed creating buffer");

  log_debug("Changing two elements");
  size_t   i0   = 2;
  size_t   i1   = 6;
  uint32_t new0 = 20;
  uint32_t new1 = 1881;
  OTTO_CALL(otto_buffer_set(&buf, i0, &new0), "Failed ");
  OTTO_CALL(otto_buffer_set(&buf, i1, &new1), "Failed ");

  log_debug("Checking elements after setting");
  uint32_t val;
  for (uint32_t i = 0; i < len; i++) {
    OTTO_CALL(otto_buffer_get(&buf, i, &val), "Failed getting buf[%d]", i);
    if (i == i0) {
      OTTO_ASSERT_EQI(val, new0);
    } else if (i == i1) {
      OTTO_ASSERT_EQI(val, new1);
    } else {
      OTTO_ASSERT_EQI(val, data[i]);
    }
  }

  log_debug("Checking border scenarios");
  if (otto_buffer_set(&buf, len - 1, NULL).status != OTTO_FAILURE) {
    log_fatal("Setting element %d did not fail", len - 1);
    return OTTO_STATUS_FAILURE("Expected fail setting element");
  }

  // Check that the failed call to otto_buffer_get did not accidentally
  // modify val
  OTTO_ASSERT_EQI(val, data[len - 1]);
  return OTTO_STATUS_SUCCESS;
}

otto_status_t test_methods_set_i8(void) {
  otto_buffer_t buf;
  int8_t        data[] = {7, 6, 5, 4, 3, 2, 1, 0};
  size_t        len    = 8;

  log_debug("Creating buffer");
  OTTO_CALL(otto_buffer_from_array(data, len, sizeof(int8_t), &buf), "Failed creating buffer");

  log_debug("Changing two elements");
  size_t i0   = 2;
  size_t i1   = 6;
  int8_t new0 = 20;
  int8_t new1 = -125;
  otto_buffer_set_i8(&buf, i0, new0);
  otto_buffer_set_i8(&buf, i1, new1);

  log_debug("Checking elements after setting");
  int8_t val;
  for (uint32_t i = 0; i < len; i++) {
    val = otto_buffer_get_i8(&buf, i);
    if (i == i0) {
      OTTO_ASSERT_EQI(val, new0);
    } else if (i == i1) {
      OTTO_ASSERT_EQI(val, new1);
    } else {
      OTTO_ASSERT_EQI(val, data[i]);
    }
  }

  log_debug("Checking border scenarios");
  if (otto_buffer_set(&buf, len - 1, NULL).status != OTTO_FAILURE) {
    log_fatal("Setting element %d did not fail", len - 1);
    return OTTO_STATUS_FAILURE("Expected fail setting element");
  }

  // Check that the failed call to otto_buffer_get did not accidentally
  // modify val
  OTTO_ASSERT_EQI(val, data[len - 1]);
  return OTTO_STATUS_SUCCESS;
}

otto_status_t test_methods_set_i16(void) {
  otto_buffer_t buf;
  int16_t       data[] = {7, 6, 5, 4, 3, 2, 1, 0};
  size_t        len    = 8;

  log_debug("Creating buffer");
  OTTO_CALL(otto_buffer_from_array(data, len, sizeof(int16_t), &buf), "Failed creating buffer");

  log_debug("Changing two elements");
  size_t  i0   = 2;
  size_t  i1   = 6;
  int16_t new0 = 20;
  int16_t new1 = 254;
  otto_buffer_set_i16(&buf, i0, new0);
  otto_buffer_set_i16(&buf, i1, new1);

  log_debug("Checking elements after setting");
  int16_t val;
  for (uint32_t i = 0; i < len; i++) {
    val = otto_buffer_get_i16(&buf, i);
    if (i == i0) {
      OTTO_ASSERT_EQI(val, new0);
    } else if (i == i1) {
      OTTO_ASSERT_EQI(val, new1);
    } else {
      OTTO_ASSERT_EQI(val, data[i]);
    }
  }

  log_debug("Checking border scenarios");
  if (otto_buffer_set(&buf, len - 1, NULL).status != OTTO_FAILURE) {
    log_fatal("Setting element %d did not fail", len - 1);
    return OTTO_STATUS_FAILURE("Expected fail setting element");
  }

  // Check that the failed call to otto_buffer_get did not accidentally
  // modify val
  OTTO_ASSERT_EQI(val, data[len - 1]);
  return OTTO_STATUS_SUCCESS;
}

otto_status_t test_methods_set_i32(void) {
  otto_buffer_t buf;
  int32_t       data[] = {7, 6, 5, 4, 3, 2, 1, 0};
  size_t        len    = 8;

  log_debug("Creating buffer");
  OTTO_CALL(otto_buffer_from_array(data, len, sizeof(int32_t), &buf), "Failed creating buffer");

  log_debug("Changing two elements");
  size_t  i0   = 2;
  size_t  i1   = 6;
  int32_t new0 = 20;
  int32_t new1 = 254;
  otto_buffer_set_i32(&buf, i0, new0);
  otto_buffer_set_i32(&buf, i1, new1);

  log_debug("Checking elements after setting");
  int32_t val;
  for (uint32_t i = 0; i < len; i++) {
    val = otto_buffer_get_i32(&buf, i);
    if (i == i0) {
      OTTO_ASSERT_EQI(val, new0);
    } else if (i == i1) {
      OTTO_ASSERT_EQI(val, new1);
    } else {
      OTTO_ASSERT_EQI(val, data[i]);
    }
  }

  log_debug("Checking border scenarios");
  if (otto_buffer_set(&buf, len - 1, NULL).status != OTTO_FAILURE) {
    log_fatal("Setting element %d did not fail", len - 1);
    return OTTO_STATUS_FAILURE("Expected fail setting element");
  }

  // Check that the failed call to otto_buffer_get did not accidentally
  // modify val
  OTTO_ASSERT_EQI(val, data[len - 1]);
  return OTTO_STATUS_SUCCESS;
}

otto_status_t test_methods_set_i64(void) {
  otto_buffer_t buf;
  int64_t       data[] = {7, 6, 5, 4, 3, 2, 1, 0};
  size_t        len    = 8;

  log_debug("Creating buffer");
  OTTO_CALL(otto_buffer_from_array(data, len, sizeof(int64_t), &buf), "Failed creating buffer");

  log_debug("Changing two elements");
  size_t  i0   = 2;
  size_t  i1   = 6;
  int64_t new0 = 20;
  int64_t new1 = 254;
  otto_buffer_set_i64(&buf, i0, new0);
  otto_buffer_set_i64(&buf, i1, new1);

  log_debug("Checking elements after setting");
  int64_t val;
  for (uint32_t i = 0; i < len; i++) {
    val = otto_buffer_get_i64(&buf, i);
    if (i == i0) {
      OTTO_ASSERT_EQI(val, new0);
    } else if (i == i1) {
      OTTO_ASSERT_EQI(val, new1);
    } else {
      OTTO_ASSERT_EQI(val, data[i]);
    }
  }

  log_debug("Checking border scenarios");
  if (otto_buffer_set(&buf, len - 1, NULL).status != OTTO_FAILURE) {
    log_fatal("Setting element %d did not fail", len - 1);
    return OTTO_STATUS_FAILURE("Expected fail setting element");
  }

  // Check that the failed call to otto_buffer_get did not accidentally
  // modify val
  OTTO_ASSERT_EQI(val, data[len - 1]);
  return OTTO_STATUS_SUCCESS;
}

otto_status_t test_methods_set_u8(void) {
  otto_buffer_t buf;
  uint8_t       data[] = {7, 6, 5, 4, 3, 2, 1, 0};
  size_t        len    = 8;

  log_debug("Creating buffer");
  OTTO_CALL(otto_buffer_from_array(data, len, sizeof(uint8_t), &buf), "Failed creating buffer");

  log_debug("Changing two elements");
  size_t  i0   = 2;
  size_t  i1   = 6;
  uint8_t new0 = 20;
  uint8_t new1 = 254;
  otto_buffer_set_u8(&buf, i0, new0);
  otto_buffer_set_u8(&buf, i1, new1);

  log_debug("Checking elements after setting");
  uint8_t val;
  for (uint32_t i = 0; i < len; i++) {
    val = otto_buffer_get_u8(&buf, i);
    if (i == i0) {
      OTTO_ASSERT_EQI(val, new0);
    } else if (i == i1) {
      OTTO_ASSERT_EQI(val, new1);
    } else {
      OTTO_ASSERT_EQI(val, data[i]);
    }
  }

  log_debug("Checking border scenarios");
  if (otto_buffer_set(&buf, len - 1, NULL).status != OTTO_FAILURE) {
    log_fatal("Setting element %d did not fail", len - 1);
    return OTTO_STATUS_FAILURE("Expected fail setting element");
  }

  // Check that the failed call to otto_buffer_get did not accidentally
  // modify val
  OTTO_ASSERT_EQI(val, data[len - 1]);
  return OTTO_STATUS_SUCCESS;
}

otto_status_t test_methods_set_u16(void) {
  otto_buffer_t buf;
  uint16_t      data[] = {7, 6, 5, 4, 3, 2, 1, 0};
  size_t        len    = 8;

  log_debug("Creating buffer");
  OTTO_CALL(otto_buffer_from_array(data, len, sizeof(uint16_t), &buf), "Failed creating buffer");

  log_debug("Changing two elements");
  size_t   i0   = 2;
  size_t   i1   = 6;
  uint16_t new0 = 20;
  uint16_t new1 = 254;
  otto_buffer_set_u16(&buf, i0, new0);
  otto_buffer_set_u16(&buf, i1, new1);

  log_debug("Checking elements after setting");
  uint16_t val;
  for (uint32_t i = 0; i < len; i++) {
    val = otto_buffer_get_u16(&buf, i);
    if (i == i0) {
      OTTO_ASSERT_EQI(val, new0);
    } else if (i == i1) {
      OTTO_ASSERT_EQI(val, new1);
    } else {
      OTTO_ASSERT_EQI(val, data[i]);
    }
  }

  log_debug("Checking border scenarios");
  if (otto_buffer_set(&buf, len - 1, NULL).status != OTTO_FAILURE) {
    log_fatal("Setting element %d did not fail", len - 1);
    return OTTO_STATUS_FAILURE("Expected fail setting element");
  }

  // Check that the failed call to otto_buffer_get did not accidentally
  // modify val
  OTTO_ASSERT_EQI(val, data[len - 1]);
  return OTTO_STATUS_SUCCESS;
}

otto_status_t test_methods_set_u32(void) {
  otto_buffer_t buf;
  uint32_t      data[] = {7, 6, 5, 4, 3, 2, 1, 0};
  size_t        len    = 8;

  log_debug("Creating buffer");
  OTTO_CALL(otto_buffer_from_array(data, len, sizeof(uint32_t), &buf), "Failed creating buffer");

  log_debug("Changing two elements");
  size_t   i0   = 2;
  size_t   i1   = 6;
  uint32_t new0 = 20;
  uint32_t new1 = 254;
  otto_buffer_set_u32(&buf, i0, new0);
  otto_buffer_set_u32(&buf, i1, new1);

  log_debug("Checking elements after setting");
  uint32_t val;
  for (uint32_t i = 0; i < len; i++) {
    val = otto_buffer_get_u32(&buf, i);
    if (i == i0) {
      OTTO_ASSERT_EQI(val, new0);
    } else if (i == i1) {
      OTTO_ASSERT_EQI(val, new1);
    } else {
      OTTO_ASSERT_EQI(val, data[i]);
    }
  }

  log_debug("Checking border scenarios");
  if (otto_buffer_set(&buf, len - 1, NULL).status != OTTO_FAILURE) {
    log_fatal("Setting element %d did not fail", len - 1);
    return OTTO_STATUS_FAILURE("Expected fail setting element");
  }

  // Check that the failed call to otto_buffer_get did not accidentally
  // modify val
  OTTO_ASSERT_EQI(val, data[len - 1]);
  return OTTO_STATUS_SUCCESS;
}

otto_status_t test_methods_set_u64(void) {
  otto_buffer_t buf;
  uint64_t      data[] = {7, 6, 5, 4, 3, 2, 1, 0};
  size_t        len    = 8;

  log_debug("Creating buffer");
  OTTO_CALL(otto_buffer_from_array(data, len, sizeof(uint64_t), &buf), "Failed creating buffer");

  log_debug("Changing two elements");
  size_t   i0   = 2;
  size_t   i1   = 6;
  uint64_t new0 = 20;
  uint64_t new1 = 254;
  otto_buffer_set_u64(&buf, i0, new0);
  otto_buffer_set_u64(&buf, i1, new1);

  log_debug("Checking elements after setting");
  uint64_t val;
  for (uint32_t i = 0; i < len; i++) {
    val = otto_buffer_get_u64(&buf, i);
    if (i == i0) {
      OTTO_ASSERT_EQI(val, new0);
    } else if (i == i1) {
      OTTO_ASSERT_EQI(val, new1);
    } else {
      OTTO_ASSERT_EQI(val, data[i]);
    }
  }

  log_debug("Checking border scenarios");
  if (otto_buffer_set(&buf, len - 1, NULL).status != OTTO_FAILURE) {
    log_fatal("Setting element %d did not fail", len - 1);
    return OTTO_STATUS_FAILURE("Expected fail setting element");
  }

  // Check that the failed call to otto_buffer_get did not accidentally
  // modify val
  OTTO_ASSERT_EQI(val, data[len - 1]);
  return OTTO_STATUS_SUCCESS;
}

otto_status_t test_methods_set_f(void) {
  otto_buffer_t buf;
  float         data[] = {7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0, 0.0};
  size_t        len    = 8;

  log_debug("Creating buffer");
  OTTO_CALL(otto_buffer_from_array(data, len, sizeof(float), &buf), "Failed creating buffer");

  log_debug("Changing two elements");
  size_t i0   = 2;
  size_t i1   = 6;
  float  new0 = 20.0;
  float  new1 = 254.0;
  otto_buffer_set_f(&buf, i0, new0);
  otto_buffer_set_f(&buf, i1, new1);

  log_debug("Checking elements after setting");
  float val;
  for (uint32_t i = 0; i < len; i++) {
    val = otto_buffer_get_f(&buf, i);
    if (i == i0) {
      OTTO_ASSERT_EQI(val, new0);
    } else if (i == i1) {
      OTTO_ASSERT_EQI(val, new1);
    } else {
      OTTO_ASSERT_EQI(val, data[i]);
    }
  }

  log_debug("Checking border scenarios");
  if (otto_buffer_set(&buf, len - 1, NULL).status != OTTO_FAILURE) {
    log_fatal("Setting element %d did not fail", len - 1);
    return OTTO_STATUS_FAILURE("Expected fail setting element");
  }

  // Check that the failed call to otto_buffer_get did not accidentally
  // modify val
  OTTO_ASSERT_EQI(val, data[len - 1]);
  return OTTO_STATUS_SUCCESS;
}

otto_status_t test_methods_set_d(void) {
  otto_buffer_t buf;
  double        data[] = {7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0, 0.0};
  size_t        len    = 8;

  log_debug("Creating buffer");
  OTTO_CALL(otto_buffer_from_array(data, len, sizeof(double), &buf), "Failed creating buffer");

  log_debug("Changing two elements");
  size_t i0   = 2;
  size_t i1   = 6;
  double new0 = 20.0;
  double new1 = 254.0;
  otto_buffer_set_d(&buf, i0, new0);
  otto_buffer_set_d(&buf, i1, new1);

  log_debug("Checking elements after setting");
  double val;
  for (uint32_t i = 0; i < len; i++) {
    val = otto_buffer_get_d(&buf, i);
    if (i == i0) {
      OTTO_ASSERT_EQI(val, new0);
    } else if (i == i1) {
      OTTO_ASSERT_EQI(val, new1);
    } else {
      OTTO_ASSERT_EQI(val, data[i]);
    }
  }

  log_debug("Checking border scenarios");
  if (otto_buffer_set(&buf, len - 1, NULL).status != OTTO_FAILURE) {
    log_fatal("Setting element %d did not fail", len - 1);
    return OTTO_STATUS_FAILURE("Expected fail setting element");
  }

  // Check that the failed call to otto_buffer_get did not accidentally
  // modify val
  OTTO_ASSERT_EQI(val, data[len - 1]);
  return OTTO_STATUS_SUCCESS;
}

int main(void) {
  OTTO_CALL_TEST(test_methods_get);
  OTTO_CALL_TEST(test_methods_get_i8);
  OTTO_CALL_TEST(test_methods_get_i16);
  OTTO_CALL_TEST(test_methods_get_i32);
  OTTO_CALL_TEST(test_methods_get_i64);
  OTTO_CALL_TEST(test_methods_get_u8);
  OTTO_CALL_TEST(test_methods_get_u16);
  OTTO_CALL_TEST(test_methods_get_u32);
  OTTO_CALL_TEST(test_methods_get_u64);
  OTTO_CALL_TEST(test_methods_get_f);
  OTTO_CALL_TEST(test_methods_get_d);

  OTTO_CALL_TEST(test_methods_set);
  OTTO_CALL_TEST(test_methods_set_i8);
  OTTO_CALL_TEST(test_methods_set_i16);
  OTTO_CALL_TEST(test_methods_set_i32);
  OTTO_CALL_TEST(test_methods_set_i64);
  OTTO_CALL_TEST(test_methods_set_u8);
  OTTO_CALL_TEST(test_methods_set_u16);
  OTTO_CALL_TEST(test_methods_set_u32);
  OTTO_CALL_TEST(test_methods_set_u64);
  OTTO_CALL_TEST(test_methods_set_f);
  OTTO_CALL_TEST(test_methods_set_d);

  return TEST_PASS;
}
