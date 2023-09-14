#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Enum representing the status of an operation.
 */
typedef enum otto_status {
  OTTO_STATUS_FAILURE = 0,
  OTTO_STATUS_SUCCESS = 1,
} otto_status_t;

#ifdef __cplusplus
}
#endif
