from enum import Enum

from otto_utils.logger import LOGGER
import otto_ffi as _
from _otto import ffi, lib as _ottol

from otto.exceptions import OttoException


class Status(Enum):
    SUCCESS = _ottol.OTTO_STATUS_SUCCESS
    FAILURE = _ottol.OTTO_STATUS_FAILURE


def ffi_call(val, msg: str = None, *, exception_cls=OttoException):
    if val == Status.FAILURE.value:
        LOGGER.error(
            f"Found a failed otto call {'' if msg is None else f'\'{msg}\''}")
        if msg is None:
            raise exception_cls("otto call failed")
        else:
            raise exception_cls(msg)
