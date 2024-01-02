#pragma once

#include "log.h"
#include <otto/cl/cl.h>
#include <otto/status.h>

const char *get_cl_error_msg(cl_int err);

#define OTTO_CALL(x, msg, ...)                                                 \
  if (x != OTTO_SUCCESS) {                                                     \
    log_error(msg, #__VA_ARGS__);                                              \
    return OTTO_FAILURE;                                                       \
  }

#define OTTO_CL_CALL(x, msg, ...)                                              \
  {                                                                            \
    cl_int err_ = x;                                                           \
    if (err_ != CL_SUCCESS) {                                                  \
      const char *err_str = get_cl_error_msg(err_);                            \
      log_error(msg, #__VA_ARGS__);                                            \
      log_error("Found CL error %d '%s'", err_, err_str);                      \
      return OTTO_FAILURE;                                                     \
    }                                                                          \
  }
