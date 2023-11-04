from dataclasses import dataclass, asdict
from enum import Enum
from typing import Hashable
from threading import Lock

from otto_utils.logger import LOGGER
import otto_ffi as _
from _otto import ffi, lib as _ottol

from otto.cl.device import Device
from otto.exceptions import OttoException, HyperparametersException
from otto.status import ffi_call


GMEM_SIZE = 8


class Kernels(Enum):
    NONE = _ottol.OTTO_KERNELS_NONE
    ALL = _ottol.OTTO_KERNELS_ALL
    CORE = _ottol.OTTO_KERNELS_CORE


@dataclass
class KernelArgs:
    work_dim: int
    global_size: int
    local_size: int


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
            else:
                LOGGER.debug(
                    f"Getting existing runtime instance, id {id_hash}"
                )
        return cls._instances[cls][id_hash]


class Runtime(metaclass=_RuntimeMeta):
    __slots__ = ("_id", "_cdata", "_ctx_props",
                 "_q_props", "_dev", "_kernel_ht")

    cls_device = Device.CPU
    cls_kernels = Kernels.CORE
    hparams = None

    def __init__(self, device: Device = None, kernels: Kernels = None, ctx_props=None, queue_props=None, kernel_build_options=None, *, instance_id=None) -> None:
        # TODO: Add option to specify context and queue properties
        LOGGER.info("Creating new runtime")
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

        if device is None:
            self._dev = self.cls_device
        else:
            self._dev = device

        if kernels is None:
            otto_kernels = self.cls_kernels
        else:
            otto_kernels = kernels
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

        if otto_kernels is not Kernels.NONE:
            LOGGER.info("Loading kernels '%s'", otto_kernels.name)
            if kernel_build_options is None:
                kernel_build_options = ""
            kernel_build_options = ffi.new(
                "const char []", kernel_build_options.encode())
            ffi_call(
                _ottol.otto_runtime_load_kernels(
                    self._cdata, otto_kernels.value, kernel_build_options),
                "Failed loading kernels",
            )

    def __del__(self):
        try:
            ffi_call(_ottol.otto_runtime_cleanup(
                self._cdata), "Failed cleanup")
        except OttoException as e:
            LOGGER.error(f"Cleaning up runtime failed with exception '{e}'")

    def call_kernel(self, name: str, *args, hparams: KernelArgs | dict = None):
        if hparams is None:
            if self.hparams is None:
                raise HyperparametersException(
                    "Hyperparameters were not given"
                )
            hparams = self.hparams
        if isinstance(hparams, KernelArgs):
            hparams = asdict(hparams)

        if "global_size" not in hparams:
            hparams["global_size"] = args[0].capacity

        hparams_ptr = ffi.new("otto_kernel_args_t *", hparams)
        kernel_args = sum([[GMEM_SIZE, a] for a in args], [])
        _ottol.otto_runtime_call_kernel(self._cdata, ffi.new(
            "const char[]", name.encode()), hparams_ptr, *kernel_args)

    def call_binop_kernel(self, name: str, lhs, rhs, out, hparams: KernelArgs | dict = None):
        if hparams is None:
            if self.hparams is None:
                raise HyperparametersException(
                    "Hyperparameters were not given"
                )
            hparams = self.hparams
        if isinstance(hparams, KernelArgs):
            hparams = asdict(hparams)

        if "global_size" not in hparams:
            hparams["global_size"] = lhs.capacity
        hparams_ptr = ffi.new("otto_kernel_args_t *", hparams)

        _ottol.otto_runtime_call_kernel_binop(self._cdata, ffi.new(
            "const char[]", name.encode()), hparams_ptr, lhs._cdata, rhs._cdata, out._cdata)

    def call_binop_kernel_no_out(self, name: str, lhs, rhs, hparams: KernelArgs | dict = None):
        out = lhs.create_from().set_write().to_device(self)
        self.call_binop_kernel(name, lhs, rhs, out, hparams)
        return out
