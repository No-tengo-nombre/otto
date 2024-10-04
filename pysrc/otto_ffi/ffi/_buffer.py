CTYPEDEF = """
typedef struct otto_buffer {
  void *data;
  cl_mem gmem;
  size_t data_size;
  size_t len;
  size_t capacity;
  otto_device_t device;
  const otto_runtime_t *ctx;
  cl_mem_flags flags;
} otto_buffer_t;
"""

CDEF = """
otto_status_t otto_buffer_new(const size_t data_size, otto_buffer_t *out);
otto_status_t otto_buffer_zero(const size_t len, const size_t data_size,
                               otto_buffer_t *out);
otto_status_t otto_buffer_with_capacity(const size_t capacity,
                                        const size_t data_size,
                                        otto_buffer_t *out);
otto_status_t otto_buffer_from_array(const void *data, const size_t len,
                                     const size_t data_size,
                                     otto_buffer_t *out);

otto_status_t otto_buffer_cleanup(const otto_buffer_t *const buf);

otto_status_t otto_buffer_get(const otto_buffer_t *buf, const size_t i,
                              void *out);
otto_status_t otto_buffer_set(otto_buffer_t *buf, const size_t i,
                              const void *src);
otto_status_t otto_buffer_resize(otto_buffer_t *buf, const size_t new_capacity);
otto_status_t otto_buffer_push(otto_buffer_t *buf, const void *src);
otto_status_t otto_buffer_extend_array(otto_buffer_t *buf, const void *src,
                                       const size_t len);

otto_status_t otto_buffer_setread(otto_buffer_t *buf);
otto_status_t otto_buffer_setwrite(otto_buffer_t *buf);
otto_status_t otto_buffer_setreadwrite(otto_buffer_t *buf);

otto_status_t otto_buffer_todevice_mode(otto_buffer_t *buf,
                                        const otto_runtime_t *ctx,
                                        const cl_mem_flags flags);
otto_status_t otto_buffer_todevice(otto_buffer_t *buf,
                                   const otto_runtime_t *ctx);
otto_status_t otto_buffer_todevice_read(otto_buffer_t *buf,
                                        const otto_runtime_t *ctx);
otto_status_t otto_buffer_todevice_write(otto_buffer_t *buf,
                                         const otto_runtime_t *ctx);
otto_status_t otto_buffer_tohost(otto_buffer_t *buf, uint64_t total);
"""
