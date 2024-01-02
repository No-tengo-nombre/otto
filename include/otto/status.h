#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Enum representing the status of an operation.
 */
typedef enum otto_status {
  OTTO_SUCCESS = 0,
  OTTO_FAILURE = 1,
  OTTO_ERR_ARGS = 2,
} otto_status_t;

#ifdef __cplusplus
}
#endif
