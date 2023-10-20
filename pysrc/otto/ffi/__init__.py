"""FFI for the otto C functions"""

from otto.defs import PYBUILD_PATH
from otto.ffi.builder import ffi_builder


def build():
    ffi_builder.compile(tmpdir=PYBUILD_PATH, verbose=True)
