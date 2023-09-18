#pragma once

#include <otto/cl/cl.h>
#include <otto/status.h>
#include "vendor/log.h"

#define CALL_I(x, msg, ...)                                                      \
  if (x != OTTO_STATUS_SUCCESS) {                                              \
    logi_fatal(msg, ##__VA_ARGS__);                                             \
    return 1;                                                                  \
  }

#define CL_CALL_I(x, msg, ...)                                                   \
  {                                                                            \
    cl_int err_ = x;                                                           \
    if (err_ != CL_SUCCESS) {                                                  \
      logi_fatal(msg, ##__VA_ARGS__);                                           \
      return 1;                                                                \
    }                                                                          \
  }
