"""Build the ffi"""

import cffi
from pathlib import Path

from otto.ffi.config import CHEADER_PATH, CLIB_DEBUG_PATH
from otto.ffi import devices, status, vector, cl


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


ffi_builder = cffi.FFI()
ffi_builder.cdef(FFI_CDEF)

# _otto = ffi_builder.set_source("_otto", str(Path.joinpath(CHEADER_PATH, "otto", "otto.h")), libraries=["otto_core"], include_dirs=[str(CLIB_DEBUG_PATH)])
_otto = ffi_builder.set_source("_otto", LIB_SOURCE, libraries=["otto_core"], include_dirs=[str(CLIB_DEBUG_PATH)])
