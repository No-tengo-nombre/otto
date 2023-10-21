"""Build the ffi"""

import os
import sys
import shutil
from pathlib import Path

import cffi

src_path = Path.joinpath(Path(__file__).parent, "pysrc")
os.environ["PYTHONPATH"] = str(src_path)
sys.path.append(str(src_path))

from otto_ffi.config import INCLUDE_DIRS, CLIB_DEBUG_PATH, CBIN_DEBUG_PATH, CLIB_RELEASE_PATH, CBIN_RELEASE_PATH, CLIB_DLL_NAMES, CLIB_SO_NAMES, SHLIB_PATH
from otto_ffi.ffi import devices, status, vector, cl


# Manually copy the corresponding shared libraries
if os.path.exists(str(Path.joinpath(CLIB_RELEASE_PATH, CLIB_SO_NAMES[0]))):
    path = CLIB_RELEASE_PATH
    files = CLIB_SO_NAMES
elif os.path.exists(str(Path.joinpath(CLIB_DEBUG_PATH, CLIB_SO_NAMES[0]))):
    path = CLIB_DEBUG_PATH
    files = CLIB_SO_NAMES
elif os.path.exists(str(Path.joinpath(CBIN_RELEASE_PATH, CLIB_DLL_NAMES[0]))):
    path = CBIN_RELEASE_PATH
    files = CLIB_DLL_NAMES
elif os.path.exists(str(Path.joinpath(CBIN_DEBUG_PATH, CLIB_DLL_NAMES[0]))):
    path = CBIN_DEBUG_PATH
    files = CLIB_DLL_NAMES

for f in files:
    shutil.copyfile(str(Path.joinpath(path, f)), str(Path.joinpath(SHLIB_PATH, f)))


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

# TODO: Make this builder run automatically and move files appropriately
