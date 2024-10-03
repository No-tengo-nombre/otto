#include <otto/status.h>

otto_status_t otto_status_new(enum otto_status status, const char *msg) {
  otto_status_t result = {
      .status = status,
      .msg    = msg,
  };
  return result;
}
