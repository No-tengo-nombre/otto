CTYPEDEF = """
enum otto_status {
  OTTO_SUCCESS = 0,
  OTTO_FAILURE = 1,
};

typedef struct otto_status_msg {
  enum otto_status status;
  const char *msg;
} otto_status_t;
"""

CDEF = """
otto_status_t otto_status_new(enum otto_status status, const char *msg);
"""
