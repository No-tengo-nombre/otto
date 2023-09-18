#pragma once

#include <otto/cl/cl.h>
#include <otto/status.h>
#include "log.h"

#define CALL(x, msg, ...)                                                      \
  if (x != OTTO_STATUS_SUCCESS) {                                              \
    log_fatal(msg, ##__VA_ARGS__);                                             \
    return 1;                                                                  \
  }

#define CL_CALL(x, msg, ...)                                                   \
  {                                                                            \
    cl_int err_ = x;                                                           \
    if (err_ != CL_SUCCESS) {                                                  \
      log_fatal(msg, ##__VA_ARGS__);                                           \
      return 1;                                                                \
    }                                                                          \
  }
