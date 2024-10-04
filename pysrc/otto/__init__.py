import otto_ffi

from . import dtypes, cl
from .exceptions import OttoException
from .status import Status, ffi_call
from .buffer import Buffer, MemFlags

# TODO: Wrap the entire library
