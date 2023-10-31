#pragma once

#include "log.h"
#include <otto/cl/cl.h>
#include <otto/status.h>

const char *get_cl_error_msg(cl_int err);

#define OTTO_CALL(x, msg, ...)                                                 \
  if (x != OTTO_STATUS_SUCCESS) {                                              \
    log_error(msg, #__VA_ARGS__);                                              \
    return OTTO_STATUS_FAILURE;                                                \
  }

#define OTTO_CL_CALL(x, msg, ...)                                              \
  {                                                                            \
    cl_int err_ = x;                                                           \
    if (err_ != CL_SUCCESS) {                                                  \
      log_error(msg " [%d](%s)", #__VA_ARGS__, err_, get_cl_error_msg(err_));  \
      return OTTO_STATUS_FAILURE;                                              \
    }                                                                          \
  }
