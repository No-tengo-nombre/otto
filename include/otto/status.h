#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Enum representing the status of an operation.
 */
enum otto_status {
  OTTO_SUCCESS = 0,
  OTTO_FAILURE = 1,
};

typedef struct otto_status_msg {
  enum otto_status status;
  const char *msg;
} otto_status_t;

otto_status_t otto_status_new(enum otto_status status, const char *msg);

#define OTTO_STATUS_SUCCESS otto_status_new(OTTO_SUCCESS, NULL)
#define OTTO_STATUS_FAILURE(msg) otto_status_new(OTTO_FAILURE, msg)

#ifdef __cplusplus
}
#endif
