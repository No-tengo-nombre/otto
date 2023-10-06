cdef extern from "otto/vector.h":
    ctypedef enum otto_status_t:
        pass
    ctypedef struct otto_vector_t:
        pass

    otto_status_t otto_vector_new(size_t data_size, otto_vector_t *out)
    otto_status_t otto_vector_zero(size_t len, size_t data_size, otto_vector_t *out)

    otto_status_t otto_vector_get(otto_vector_t *vec, size_t i, void *out)

    otto_status_t otto_vector_push(otto_vector_t *vec, void *src)


class Vector:
    def __init__(self, otto_vector_t *vec, size_t data_size, dtype):
        self.vec = vec
        self.dtype = dtype
        otto_vector_new(data_size, vec)

    def get(self, size_t i):
        cdef self.dtype val = 0
        otto_vector_get(self.vec, i, &val)
        return val

    def push(self, void *val):
        otto_vector_push(self.vec, val)
