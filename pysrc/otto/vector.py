import sys

from otto.dtypes import DataType

import otto_ffi as _
from _otto import ffi, lib as _ottol


class Vector:
    def __init__(self, dtype: DataType):
        self._cdata = ffi.new("otto_vector_t *")
        self._dtype = dtype
        self._dsize = self._dtype.size

    @classmethod
    def new(cls, dtype: DataType):
        vec = cls(dtype)
        _ottol.otto_vector_new(vec._dsize, vec._cdata)
        return vec

    @classmethod
    def zero(cls, size: int, dtype: DataType):
        vec = cls(dtype)
        _ottol.otto_vector_zero(size, vec._dsize, vec._cdata)
        return vec

    @classmethod
    def with_capacity(cls, capacity: int, dtype: DataType):
        vec = cls(dtype)
        _ottol.otto_vector_with_capacity(capacity, vec._dsize, vec._cdata)
        return vec

    @property
    def len(self) -> int:
        return self._cdata.len

    @property
    def capacity(self) -> int:
        return self._cdata.capacity

    @property
    def data_size(self) -> int:
        return self._cdata.data_size

    # TODO: Implement creation from arrays and lists

    def __del__(self):
        _ottol.otto_vector_cleanup(self._cdata)

    def __str__(self) -> str:
        return f"Vector<{self._dtype.name}>[{self.len}/{self.capacity}]"

    def string(self) -> str:
        return self.__str__()

    def get(self, idx: int):
        val = ffi.new(f"{self._dtype.long_name} *")
        if idx >= self.len:
            raise IndexError(f"index {idx} out of range for vector of len {self.len}")
        if idx < -self.len:
            raise IndexError(f"index {idx % self.len} out of range for vector of len {self.len}")
        idx = idx % self.len
        _ottol.otto_vector_get(self._cdata, idx, val)
        return val[0]
