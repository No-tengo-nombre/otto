CTYPEDEF = """
typedef ... otto_vector_t;
"""

CDEF = """
otto_status_t otto_vector_new(const size_t data_size, otto_vector_t *out);
otto_status_t otto_vector_zero(const size_t len, const size_t data_size,
                               otto_vector_t *out);
otto_status_t otto_vector_with_capacity(const size_t capacity,
                                        const size_t data_size,
                                        otto_vector_t *out);
otto_status_t otto_vector_from_array(const void *data, const size_t len,
                                     const size_t data_size,
                                     otto_vector_t *out);


otto_status_t otto_vector_cleanup(const otto_vector_t *const vec);


otto_status_t otto_vector_get(const otto_vector_t *vec, const size_t i,
                              void *out);
otto_status_t otto_vector_set(otto_vector_t *vec, const size_t i,
                              const void *src);
otto_status_t otto_vector_resize(otto_vector_t *vec, const size_t new_capacity);
otto_status_t otto_vector_push(otto_vector_t *vec, const void *src);
otto_status_t otto_vector_extend_array(otto_vector_t *vec, const void *src,
                                       const size_t len);


otto_status_t otto_vector_todevice(otto_vector_t *vec,
                                   const otto_runtime_t *ctx,
                                   const cl_mem_flags flags);
otto_status_t otto_vector_tohost(otto_vector_t *vec, uint64_t total);
"""
