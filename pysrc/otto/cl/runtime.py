from enum import Enum
from typing import Hashable
from threading import Lock

from otto_utils.logger import LOGGER
import otto_ffi as _
from _otto import ffi, lib as _ottol

from otto.cl.device import Device
from otto.exceptions import OttoException
from otto.status import ffi_call


class Kernels(Enum):
    ALL = _ottol.OTTO_KERNELS_ALL
    CORE = _ottol.OTTO_KERNELS_CORE


class _RuntimeMeta(type):
    _instances = {}
    _lock = Lock()

    def __call__(cls, *args, instance_id: Hashable = None, **kwargs):
        id_hash = hash(instance_id)
        with cls._lock:
            if cls not in cls._instances:
                cls._instances[cls] = {}
            if id_hash not in cls._instances[cls]:
                LOGGER.debug("Registering new runtime instance")
                instance = super().__call__(*args, **kwargs)
                cls._instances[cls][id_hash] = instance
        LOGGER.debug(f"Getting existing runtime instance, id {id_hash}")
        return cls._instances[cls][id_hash]


class Runtime(metaclass=_RuntimeMeta):
    __slots__ = ("_id", "_cdata", "_ctx_props",
                 "_q_props", "_dev", "_kernel_ht")

    def __init__(self, device: Device = Device.CPU, kernels: Kernels = None, ctx_props=None, queue_props=None, kernel_build_options=None, *, instance_id=None) -> None:
        # TODO: Add option to specify context and queue properties
        self._id = instance_id
        self._cdata = ffi.new("otto_runtime_t *")
        if ctx_props is None:
            self._ctx_props = ffi.NULL
        else:
            self._ctx_props = ctx_props
        if queue_props is None:
            self._q_props = ffi.NULL
        else:
            self._q_props = queue_props
        self._dev = device
        self._kernel_ht = ffi.NULL

        # Initialize the runtime
        # TODO: Figure out why it fails if using CPU (likely an otto error)
        LOGGER.debug("Using device %s: %d", self._dev.name, self._dev.value)
        ffi_call(_ottol.otto_runtime_new(
            self._ctx_props,
            self._q_props,
            self._dev.value,
            self._kernel_ht,
            self._cdata,
        ), "Runtime creation failed")

        if kernels is not None:
            if kernel_build_options is None:
                kernel_build_options = ""
            kernel_build_options = ffi.new(
                "const char []", kernel_build_options.encode())
            ffi_call(
                _ottol.otto_runtime_load_kernels(
                    self._cdata, kernels.value, kernel_build_options),
                "Failed loading kernels",
            )

    # TODO: Implement the rest of the methods

    def __del__(self):
        try:
            ffi_call(_ottol.otto_runtime_cleanup(
                self._cdata), "Failed cleanup")
        except OttoException as e:
            LOGGER.error(f"Cleaning up runtime failed with exception '{e}'")
