#pragma once

#include "log.h"
#include <otto/cl/cl.h>
#include <otto/status.h>

#define OTTO_CALL(x, msg, ...)                                                 \
  if (x != OTTO_STATUS_SUCCESS) {                                              \
    log_error(msg, ##__VA_ARGS__);                                             \
    return OTTO_STATUS_FAILURE;                                                \
  }

#define OTTO_CL_CALL(x, msg, ...)                                              \
  {                                                                            \
    cl_int err_ = x;                                                           \
    if (err_ != CL_SUCCESS) {                                                  \
      log_error(msg " (%d)", ##__VA_ARGS__, err_);                             \
      return OTTO_STATUS_FAILURE;                                              \
    }                                                                          \
  }
