#pragma once

#include "vendor/log.h"
#include <otto/cl/cl.h>
#include <otto/status.h>
#include <ottou/macros.h>

#define OTTO_CALL_I(x, msg, ...)                                               \
  if (x != OTTO_STATUS_SUCCESS) {                                              \
    logi_error(msg, #__VA_ARGS__);                                             \
    return OTTO_STATUS_FAILURE;                                                \
  }

#define OTTO_CL_CALL_I(x, msg, ...)                                            \
  {                                                                            \
    cl_int err_ = x;                                                           \
    if (err_ != CL_SUCCESS) {                                                  \
      logi_error(msg " [%d](%s)", #__VA_ARGS__, err_, get_cl_error_msg(err_)); \
      return OTTO_STATUS_FAILURE;                                              \
    }                                                                          \
  }
