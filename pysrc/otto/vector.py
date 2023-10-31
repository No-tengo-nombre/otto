from collections.abc import Iterator, Iterable
from functools import singledispatchmethod
from numbers import Real
from typing import List, Self

import numpy as np

from otto_utils.logger import LOGGER
import otto_ffi as _
from _otto import ffi, lib as _ottol

from otto import dtypes


class Vector[T]:
    __slots__ = ("_cdata", "_dtype", "_dsize", "_index")

    def __init__(self, dtype: dtypes.DataType):
        self._cdata = ffi.new("otto_vector_t *")
        self._dtype = dtype
        self._dsize = self._dtype.size

    @classmethod
    def empty(cls, dtype: dtypes.DataType):
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
        LOGGER.debug("Creating from list")
        # TODO: Fix the bug that happens when the first element is less general than the rest
        if dtype is None:
            if target == []:
                LOGGER.error("Found empty list, raising exception")
                raise ValueError(
                    "Expected non-empty empty list (see `Vector.empty` for empty initialization)"
                )
            dtype = dtypes.get_ctype(target[0])
            if dtype is None:
                raise TypeError(
                    f"Conversion of type '{
                        type(target[0]).__name__}' not implemented"
                )
        vec = cls(dtype)
        size = len(target)
        val = ffi.new(f"{dtype.long_name}[]", target)
        _ottol.otto_vector_from_array(val, size, dtype.size, vec._cdata)
        return vec

    @classmethod
    def from_numpy(cls, target: np.ndarray, dtype: dtypes.DataType = None):
        # TODO: Determine a more efficient way of creating from numpy array
        LOGGER.debug("Creating from numpy array")
        if dtype is None:
            dtype = dtypes._NP_TYPES.get(target.dtype.name)
            if dtype is None:
                raise TypeError(
                    f"Conversion of type 'np.{
                        target.dtype.name}' not implemented"
                )
        vec = cls(dtype)
        size = len(target.flatten())
        val = ffi.new(f"{dtype.long_name}[]", list(target.flatten()))
        _ottol.otto_vector_from_array(val, size, dtype.size, vec._cdata)
        return vec

    @classmethod
    def from_iter(cls, target: Iterator, dtype: dtypes.DataType = None):
        LOGGER.debug("Creating from iterator")
        vals = list(target)
        if dtype is None:
            if vals == []:
                LOGGER.error("Found empty iterator, raising exception")
                raise ValueError(
                    "Expected non-empty empty iterator (see `Vector.empty` for empty initialization)"
                )
            dtype = dtypes.get_ctype(vals[0])
            if dtype is None:
                raise TypeError(
                    f"Conversion of type '{
                        type(vals[0]).__name__}' not implemented"
                )
        return cls.from_list(vals, dtype)

    @singledispatchmethod
    @classmethod
    def new(cls, target, dtype: dtypes.DataType = None):
        try:
            LOGGER.warn("Trying to iterate invalid input type")
            return cls.from_iter(iter(target), dtype)
        except TypeError:
            LOGGER.error("Trying to create Vector from invalid type")
            raise TypeError(
                f"Could not create `Vector` from type '{
                    type(target).__name__}'"
            )

    @new.register(list)
    @classmethod
    def _(cls, target: list, dtype: dtypes.DataType = None):
        return cls.from_list(target, dtype)

    @new.register(np.ndarray)
    @classmethod
    def _(cls, target: np.ndarray, dtype: dtypes.DataType = None):
        return cls.from_numpy(target, dtype)

    @new.register(Iterator)
    @classmethod
    def _(cls, target: Iterator, dtype: dtypes.DataType = None):
        return cls.from_iter(target, dtype)

    @new.register(Iterable)
    @classmethod
    def _(cls, target: Iterable, dtype: dtypes.DataType = None):
        return cls.from_iter(iter(target), dtype)

    @new.register(Real)
    @classmethod
    def _(cls, target: Real, dtype: dtypes.DataType = None):
        return cls.from_list([target], dtype)

    @property
    def len(self) -> int:
        return self._cdata.len

    @property
    def capacity(self) -> int:
        return self._cdata.capacity

    @property
    def data_size(self) -> int:
        return self._cdata.data_size

    def __del__(self):
        _ottol.otto_vector_cleanup(self._cdata)

    def __str__(self) -> str:
        return f"{self._generic_str()}[{self.len}/{self.capacity}]"

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

    def __array__(self) -> np.ndarray:
        # TODO: Determine a more efficient way of converting to numpy array
        return np.array(self.to_list(), dtype=self._dtype.np)

    # TODO: Implement arithmetic operations using OpenCL kernels

    def string(self) -> str:
        return self.__str__()

    def _generic_str(self) -> str:
        return f"Vector<{self._dtype.name}>"

    def content_string(self) -> str:
        return f"{self._generic_str()}{self.to_list()}"

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

    def to_numpy(self) -> np.ndarray:
        return np.array(self)

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
