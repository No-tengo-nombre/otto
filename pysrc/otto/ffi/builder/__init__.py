"""Build the ffi"""

import cffi
from pathlib import Path

from otto.ffi.config import CLIB_DEBUG_PATH, CSOURCES_PATH
from otto.ffi import devices, status, vector, cl


# Parameters for the FFI

LIB_TYPEDEF = """
typedef ... va_list;
typedef ... cl_device_type;
typedef ... cl_mem_flags;
typedef ... cl_context_properties;
typedef ... cl_queue_properties;
"""

FFI_CDEF = f"""
// Typedefs
{LIB_TYPEDEF}
{status.CTYPEDEF}
{devices.CTYPEDEF}
{vector.CTYPEDEF}
{cl.kernel.CTYPEDEF}
{cl.program.CTYPEDEF}
{cl.runtime.CTYPEDEF}
// Function definitions
{status.CDEF}
{devices.CDEF}
{vector.CDEF}
{cl.kernel.CDEF}
{cl.program.CDEF}
{cl.runtime.CDEF}
"""

LIB_SOURCE = """
#include <otto/otto.h>
"""

LIB_SOURCES = [
    str(Path.joinpath(CSOURCES_PATH, "otto_core", "cl", "kernel.c")),
    str(Path.joinpath(CSOURCES_PATH, "otto_core", "cl", "program.c")),
    str(Path.joinpath(CSOURCES_PATH, "otto_core", "cl", "runtime_ll.c")),
    str(Path.joinpath(CSOURCES_PATH, "otto_core", "cl", "runtime.c")),
    str(Path.joinpath(CSOURCES_PATH, "otto_core", "devices.c")),
    str(Path.joinpath(CSOURCES_PATH, "otto_core", "vector_attrs.c")),
    str(Path.joinpath(CSOURCES_PATH, "otto_core", "vector_creation.c")),
    str(Path.joinpath(CSOURCES_PATH, "otto_core", "vector_dev.c")),
]

# Builder for the FFI

ffi_builder = cffi.FFI()

ffi_builder.cdef(FFI_CDEF)
ffi_builder.set_source(
    "_otto",
    LIB_SOURCE,
    sources=LIB_SOURCES,
    libraries=["otto_core"],
    include_dirs=[str(CLIB_DEBUG_PATH)],
)
