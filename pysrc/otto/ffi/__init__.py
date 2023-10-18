"""FFI for the otto C functions"""

from otto.ffi.builder import ffi_builder


def build():
    ffi_builder.compile(verbose=True)
