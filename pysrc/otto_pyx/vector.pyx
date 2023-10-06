from libc.stdlib cimport malloc, free

cdef extern from "otto/vector.h":
    ctypedef enum otto_status_t:
        pass
    ctypedef struct otto_vector_t:
        pass

    otto_status_t otto_vector_new(size_t data_size, otto_vector_t *out)
    otto_status_t otto_vector_zero(size_t len, size_t data_size, otto_vector_t *out)
    otto_status_t otto_vector_cleanup(otto_vector_t *vec)

    otto_status_t otto_vector_get(otto_vector_t *vec, size_t i, void *out)

    otto_status_t otto_vector_push(otto_vector_t *vec, void *src)


cdef class Vector:
    cdef otto_vector_t *_c_vec

    def __cinit__(self):
        self._c_vec = <otto_vector_t*>malloc(sizeof(otto_vector_t))
        # self.dtype = dtype

    def init(self, size_t data_size):
        otto_vector_new(data_size, self._c_vec)

    def __dealloc__(self):
        otto_vector_cleanup(self._c_vec)

    def get(self, i):
        cdef int val = 0
        otto_vector_get(self._c_vec, i, &val)
        # return val

    # def push(self, val):
    #     cdef void *val_p = &val
    #     otto_vector_push(self._c_vec, val_p)
