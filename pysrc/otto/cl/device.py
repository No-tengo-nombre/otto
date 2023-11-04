from enum import Enum

import otto_ffi as _
from _otto import ffi, lib as _ottol


class Device(Enum):
    CPU = _ottol.OTTO_DEVICE_CPU
    GPU = _ottol.OTTO_DEVICE_GPU
