CTYPEDEF = """
typedef ... otto_device_t;
"""

CDEF = """
otto_status_t otto_device_as_cl(const otto_device_t dev, cl_device_type *out);
otto_status_t otto_device_name(const otto_device_t dev, const char **out);
"""
