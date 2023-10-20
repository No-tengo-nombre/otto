"""Build the ffi"""

import cffi

from otto.ffi.config import INCLUDE_DIRS, CLIB_DEBUG_PATH, CBIN_DEBUG_PATH, CLIB_RELEASE_PATH, CBIN_RELEASE_PATH
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

# Builder for the FFI

ffi_builder = cffi.FFI()

ffi_builder.cdef(FFI_CDEF)
ffi_builder.set_source(
    "_otto",
    LIB_SOURCE,
    libraries=["otto_core"],
    library_dirs=[str(CBIN_DEBUG_PATH), str(CLIB_DEBUG_PATH), str(CBIN_RELEASE_PATH), str(CLIB_RELEASE_PATH)],
    include_dirs=INCLUDE_DIRS,
    # extra_link_args=["-Wl,-rpath,."],
)
