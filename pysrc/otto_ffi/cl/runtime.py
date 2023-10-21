CTYPEDEF = """
typedef ... otto_kernelht_t;
typedef ... otto_kernelll_t;
typedef ... otto_runtime_t;
"""

CDEF = """
otto_status_t otto_runtime_new(const cl_context_properties *ctx_props,
                               const cl_queue_properties *q_props,
                               const otto_device_t type,
                               otto_kernelht_t *kernel_ht, otto_runtime_t *out);

otto_status_t otto_runtime_cleanup(const otto_runtime_t *ctx);

otto_status_t otto_runtime_add_kernel(otto_runtime_t *ctx, const char *name,
                                      otto_kernel_t *kernel);
otto_status_t otto_runtime_get_kernel(const otto_runtime_t *ctx,
                                      const char *name, otto_kernel_t *out);
otto_status_t otto_runtime_vcall_kernel(const otto_runtime_t *ctx,
                                        const char *name,
                                        const otto_kernel_args_t *hparams,
                                        va_list args);
otto_status_t otto_runtime_call_kernel(const otto_runtime_t *ctx,
                                       const char *name,
                                       const otto_kernel_args_t *hparams, ...);
otto_status_t otto_runtime_call_kernel_binop(const otto_runtime_t *ctx,
                                             const char *name,
                                             const otto_kernel_args_t *hparams,
                                             const otto_vector_t *a,
                                             const otto_vector_t *b,
                                             otto_vector_t *out);
"""
