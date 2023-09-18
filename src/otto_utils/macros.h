#pragma once

#include "vendor/log.h"
#include <otto/cl/cl.h>
#include <otto/status.h>

#define OTTO_CALL_I(x, msg, ...)                                               \
  if (x != OTTO_STATUS_SUCCESS) {                                              \
    logi_fatal(msg, ##__VA_ARGS__);                                            \
    return OTTO_STATUS_FAILURE;                                                \
  }

#define OTTO_CL_CALL_I(x, msg, ...)                                            \
  {                                                                            \
    cl_int err_ = x;                                                           \
    if (err_ != CL_SUCCESS) {                                                  \
      logi_fatal(msg " (%d)", ##__VA_ARGS__, err_);                            \
      return OTTO_STATUS_FAILURE;                                              \
    }                                                                          \
  }
