from collections.abc import Iterator, Iterable
from enum import Enum
from functools import singledispatchmethod
from numbers import Real
from typing import List, Self

import numpy as np

from otto_utils.logger import LOGGER
import otto_ffi as _
from _otto import ffi, lib as _ottol

from otto import dtypes
from otto.cl.runtime import Runtime
from otto.exceptions import OttoException
from otto.status import ffi_call


class MemFlags(Enum):
    READ = _ottol.CL_MEM_READ_ONLY
    WRITE = _ottol.CL_MEM_WRITE_ONLY
    READ_WRITE = _ottol.CL_MEM_READ_WRITE


class Vector[T]:
    __slots__ = ("_cdata", "_dtype", "_dsize",
                 "_index", "ctx", "mode", "hparams")

    def __init__(self, dtype: dtypes.DataType, mode: MemFlags = MemFlags.READ):
        self._cdata = ffi.new("otto_vector_t *")
        self._dtype = dtype
        self._dsize = self._dtype.size
        self.ctx = None
        self.mode = mode

    @classmethod
    def empty(cls, dtype: dtypes.DataType, mode: MemFlags = MemFlags.READ):
        vec = cls(dtype, mode)
        ffi_call(_ottol.otto_vector_new(vec._dsize, vec._cdata),
                 "Failed creating empty vector")
        return vec

    @classmethod
    def zero(cls, size: int, dtype: dtypes.DataType, mode: MemFlags = MemFlags.READ):
        vec = cls(dtype, mode)
        ffi_call(_ottol.otto_vector_zero(size, vec._dsize, vec._cdata),
                 "Failed creating zero initialized vector")
        return vec

    @classmethod
    def with_capacity(cls, capacity: int, dtype: dtypes.DataType, mode: MemFlags = MemFlags.READ):
        vec = cls(dtype, mode)
        ffi_call(_ottol.otto_vector_with_capacity(capacity, vec._dsize,
                 vec._cdata), f"Failed creating vector with capacity {capacity}")
        return vec

    @classmethod
    def from_list(cls, target: List[T], dtype: dtypes.DataType = None, mode: MemFlags = MemFlags.READ):
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
        vec = cls(dtype, mode)
        size = len(target)
        val = ffi.new(f"{dtype.long_name}[]", target)
        ffi_call(_ottol.otto_vector_from_array(val, size, dtype.size,
                 vec._cdata), "Failed creating vector from list")
        return vec

    @classmethod
    def from_numpy(cls, target: np.ndarray, dtype: dtypes.DataType = None, mode: MemFlags = MemFlags.READ):
        # TODO: Determine a more efficient way of creating from numpy array
        LOGGER.debug("Creating from numpy array")
        if dtype is None:
            dtype = dtypes._NP_TYPES.get(target.dtype.name)
            if dtype is None:
                raise TypeError(
                    f"Conversion of type 'np.{
                        target.dtype.name}' not implemented"
                )
        vec = cls(dtype, mode)
        size = len(target.flatten())
        val = ffi.new(f"{dtype.long_name}[]", list(target.flatten()))
        ffi_call(_ottol.otto_vector_from_array(val, size, dtype.size,
                 vec._cdata), "Failed creating vector from numpy array")
        return vec

    @classmethod
    def from_iter(cls, target: Iterator, dtype: dtypes.DataType = None, mode: MemFlags = MemFlags.READ):
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
        return cls.from_list(vals, dtype, mode)

    @singledispatchmethod
    @classmethod
    def new(cls, target, dtype: dtypes.DataType = None, mode: MemFlags = MemFlags.READ):
        try:
            LOGGER.warn("Trying to iterate invalid input type")
            return cls.from_iter(iter(target), dtype, mode)
        except TypeError:
            LOGGER.error("Trying to create Vector from invalid type")
            raise TypeError(
                f"Could not create `Vector` from type '{
                    type(target).__name__}'"
            )

    @new.register(list)
    @classmethod
    def _(cls, target: list, dtype: dtypes.DataType = None, mode: MemFlags = MemFlags.READ):
        return cls.from_list(target, dtype, mode)

    @new.register(np.ndarray)
    @classmethod
    def _(cls, target: np.ndarray, dtype: dtypes.DataType = None, mode: MemFlags = MemFlags.READ):
        return cls.from_numpy(target, dtype, mode)

    @new.register(Iterator)
    @classmethod
    def _(cls, target: Iterator, dtype: dtypes.DataType = None, mode: MemFlags = MemFlags.READ):
        return cls.from_iter(target, dtype, mode)

    @new.register(Iterable)
    @classmethod
    def _(cls, target: Iterable, dtype: dtypes.DataType = None, mode: MemFlags = MemFlags.READ):
        return cls.from_iter(iter(target), dtype, mode)

    @new.register(Real)
    @classmethod
    def _(cls, target: Real, dtype: dtypes.DataType = None, mode: MemFlags = MemFlags.READ):
        return cls.from_list([target], dtype, mode)

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
        try:
            ffi_call(_ottol.otto_vector_cleanup(
                self._cdata), "Failed cleanup of vector")
        except OttoException as e:
            LOGGER.error(f"Cleaning up vector failed with exception '{e}'")

    def __str__(self) -> str:
        return self.content_string()

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

    def __add__(self, rhs):
        self._validate_runtime()
        return self.ctx.call_binop_kernel_no_out("otto_vector_add", self, rhs, None)

    def __sub__(self, rhs):
        self._validate_runtime()
        return self.ctx.call_binop_kernel_no_out("otto_vector_sub", self, rhs, None)

    def __mul__(self, rhs):
        self._validate_runtime()
        return self.ctx.call_binop_kernel_no_out("otto_vector_mul", self, rhs, None)

    def __matmul__(self, rhs):
        raise NotImplementedError("Method not implemented")

    def __truediv__(self, rhs):
        self._validate_runtime()
        return self.ctx.call_binop_kernel_no_out("otto_vector_div", self, rhs, None)

    def __floordiv__(self, rhs):
        raise NotImplementedError("Method not implemented")

    def __mod__(self, rhs):
        raise NotImplementedError("Method not implemented")

    def __divmod__(self, rhs):
        raise NotImplementedError("Method not implemented")

    def __pow__(self, rhs):
        raise NotImplementedError("Method not implemented")

    def __lshift__(self, rhs):
        raise NotImplementedError("Method not implemented")

    def __rshift__(self, rhs):
        raise NotImplementedError("Method not implemented")

    def __and__(self, rhs):
        raise NotImplementedError("Method not implemented")

    def __xor__(self, rhs):
        raise NotImplementedError("Method not implemented")

    def __or__(self, rhs):
        raise NotImplementedError("Method not implemented")

    def string(self) -> str:
        return self.__str__()

    def _generic_str(self) -> str:
        return f"Vector<{self._dtype.name}>"

    def content_string(self) -> str:
        return f"{self._generic_str()}{self.to_list()}"

    def short_string(self) -> str:
        return f"{self._generic_str()}[{self.len}/{self.capacity}]"

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
        ffi_call(_ottol.otto_vector_get(self._cdata, key, val),
                 f"Failed getting element at index {key}")
        return val[0]

    def set_key(self, key: int, value) -> None:
        key = self.validate_index(key)
        val = ffi.new(f"{self._dtype.long_name} *")
        val[0] = value
        ffi_call(_ottol.otto_vector_set(self._cdata, key, val),
                 f"Failed setting element at key {key}")

    def _validate_runtime(self):
        if self.ctx is None:
            self.ctx = Runtime()

    def to_list(self) -> List[T]:
        return list(self)

    def to_numpy(self) -> np.ndarray:
        return np.array(self)

    def resize(self, new_capacity: int) -> None:
        ffi_call(_ottol.otto_vector_resize(
            self._cdata, new_capacity), "Failed resizing")

    def push(self, element: T) -> None:
        val = ffi.new(f"{self._dtype.long_name} *")
        val[0] = element
        ffi_call(_ottol.otto_vector_push(
            self._cdata, val), "Failed pushing element")

    def extend_from_list(self, target: List[T]):
        size = len(target)
        val = ffi.new(f"{self._dtype.long_name}[]", target)
        ffi_call(_ottol.otto_vector_extend_array(
            self._cdata, val, size), "Failed extending from list")

    def extend_from_numpy(self, target: np.ndarray):
        target = target.flatten()
        size = len(target)
        val = ffi.new(f"{self._dtype.long_name}[]", target)
        ffi_call(_ottol.otto_vector_extend_array(
            self._cdata, val, size), "Failed extending from numpy array")

    def extend_from_iter(self, target: Iterable):
        self.extend_from_list(iter(target))

    @singledispatchmethod
    def extend(self, target):
        try:
            LOGGER.warn(
                "Trying to extend from invalid input type, trying as iter")
            return self.extend_from_iter(iter(target))
        except TypeError:
            LOGGER.error("Trying to extend Vector from invalid type")
            raise TypeError(
                f"Could not extend `Vector` from type '{
                    type(target).__name__}'"
            )

    @extend.register(list)
    def _(self, target):
        return self.extend_from_list(target)

    @extend.register(np.ndarray)
    def _(self, target):
        return self.extend_from_numpy(target)

    @extend.register(Iterator)
    def _(self, target):
        return self.extend_from_iter(target)

    @extend.register(Iterable)
    def _(self, target):
        return self.extend_from_iter(iter(target))

    def set_mode(self, mode: MemFlags) -> Self:
        LOGGER.debug("Setting vector as '%s'", mode.name)
        self.mode = mode
        return self

    def set_read(self) -> Self:
        return self.set_mode(MemFlags.READ)

    def set_write(self) -> Self:
        return self.set_mode(MemFlags.WRITE)

    def set_read_write(self) -> Self:
        return self.set_mode(MemFlags.READ_WRITE)

    def to_device_mode(self, ctx: Runtime = None, mode: MemFlags = None) -> Self:
        if mode is None:
            mode = self.mode

        if ctx is None:
            LOGGER.info("Getting default runtime")
            ctx = Runtime()
        self.ctx = ctx
        _ottol.otto_vector_todevice_mode(self._cdata, ctx._cdata, mode.value)
        return self

    def to_device_read(self, ctx: Runtime = None) -> Self:
        return self.to_device_mode(ctx, MemFlags.READ)

    def to_device_write(self, ctx: Runtime = None) -> Self:
        return self.to_device_mode(ctx, MemFlags.WRITE)

    def to_device_read_write(self, ctx: Runtime = None) -> Self:
        return self.to_device_mode(ctx, MemFlags.READ_WRITE)

    def to_device(self, ctx: Runtime = None) -> Self:
        return self.to_device_mode(ctx, self.mode)

    def to_host(self, total=0) -> Self:
        _ottol.otto_vector_tohost(self._cdata, total)
        return self

    def create_from(self) -> Self:
        return Vector.with_capacity(self.capacity, self._dtype, self.mode)
