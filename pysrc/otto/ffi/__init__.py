"""FFI for the otto C functions"""

from otto.ffi.builder import ffi_builder, _otto


def build():
    ffi_builder.compile()
