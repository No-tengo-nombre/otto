from typing import List, Self

from otto_utils.logger import LOGGER
import otto_ffi as _
from _otto import ffi, lib as _ottol

from otto import dtypes


# TODO: Update this class to account for the interface changes


class Vector[T]:
    __slots__ = ("_cdata", "_dtype", "_dsize", "_index")

    def __init__(self, dtype: dtypes.DataType):
        self._cdata = ffi.new("otto_vector_t *")
        self._dtype = dtype
        self._dsize = self._dtype.size

    @classmethod
    def new(cls, dtype: dtypes.DataType):
        vec = cls(dtype)
        _ottol.otto_vector_new(vec._dsize, vec._cdata)
        return vec

    @classmethod
    def zero(cls, size: int, dtype: dtypes.DataType):
        vec = cls(dtype)
        _ottol.otto_vector_zero(size, vec._dsize, vec._cdata)
        return vec

    @classmethod
    def with_capacity(cls, capacity: int, dtype: dtypes.DataType):
        vec = cls(dtype)
        _ottol.otto_vector_with_capacity(capacity, vec._dsize, vec._cdata)
        return vec

    @classmethod
    def from_list(cls, target: List[T], dtype: dtypes.DataType = None):
        if dtype is None:
            if target == []:
                LOGGER.error("Found empty list, raising exception")
                raise ValueError(
                    "Expected non-empty empty list (see `Vector.new` for empty lists)"
                )
            dtype = dtypes.get_ctype(target[0])
        vec = cls(dtype)
        size = len(target)
        val = ffi.new(f"{dtype.long_name}[]", target)
        _ottol.otto_vector_from_array(val, size, dtype.size, vec._cdata)
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

    def __getitem__(self, key: int) -> T:
        return self.get(key)

    def __setitem__(self, key: int, value: T):
        return self.set_key(key, value)

    def __iter__(self) -> Self:
        self._index = 0
        return self

    def __next__(self) -> T:
        try:
            val = self[self._index]
        except IndexError:
            raise StopIteration
        self._index += 1
        return val

    def __len__(self) -> int:
        return self.len

    # TODO: Implement arithmetic operations using OpenCL kernels

    def string(self) -> str:
        return self.__str__()

    def validate_index(self, idx: int) -> int:
        if idx >= self.len:
            raise IndexError(
                f"index {idx} out of range for vector of len {self.len}")
        if idx < -self.len:
            raise IndexError(
                f"index {idx % self.len} out of range for vector of len {self.len}")
        return idx % self.len

    def get(self, key: int) -> T:
        key = self.validate_index(key)
        val = ffi.new(f"{self._dtype.long_name} *")
        _ottol.otto_vector_get(self._cdata, key, val)
        return val[0]

    def set_key(self, key: int, value) -> None:
        key = self.validate_index(key)
        val = ffi.new(f"{self._dtype.long_name} *")
        val[0] = value
        _ottol.otto_vector_set(self._cdata, key, val)

    def to_list(self) -> List[T]:
        return list(self)

    # TODO: Implement interpreting as different collections (e.g. numpy arrays)

    def resize(self, new_capacity: int) -> None:
        _ottol.otto_vector_resize(self._cdata, new_capacity)

    def push(self, element: T) -> None:
        val = ffi.new(f"{self._dtype.long_name} *")
        val[0] = element
        _ottol.otto_vector_push(self._cdata, val)

    def extend_from_list(self, target: List[T]):
        size = len(target)
        val = ffi.new(f"{self._dtype.long_name}[]", target)
        _ottol.otto_vector_extend_array(self._cdata, val, size)
