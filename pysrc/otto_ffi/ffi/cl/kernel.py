CTYPEDEF = """
typedef ... otto_kernel_args_t;
typedef ... otto_kernel_t;
"""

CDEF = """
otto_status_t otto_kernel_new(const otto_program_t *prog, const char *name,
                              const size_t nargs, otto_runtime_t *ctx,
                              otto_kernel_t *out);

otto_status_t otto_kernel_vcall(const otto_kernel_t *ker,
                                const otto_runtime_t *ctx,
                                const otto_kernel_args_t *hparams,
                                va_list args);
otto_status_t otto_kernel_call(const otto_kernel_t *ker,
                               const otto_runtime_t *ctx,
                               const otto_kernel_args_t *hparams, ...);
otto_status_t otto_kernel_call_binop(const otto_kernel_t *ker,
                                     const otto_runtime_t *ctx,
                                     const otto_kernel_args_t *hparams,
                                     const otto_vector_t *a,
                                     const otto_vector_t *b,
                                     otto_vector_t *out);
"""
