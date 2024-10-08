"""Build the ffi"""

import os
import sys
import shutil
from pathlib import Path

import cffi

src_path = Path.joinpath(Path(__file__).parent, "pysrc")
os.environ["PYTHONPATH"] = str(src_path)
sys.path.append(str(src_path))
from otto_ffi.ffi import _devices, _status, _buffer, _cl
from otto_ffi.config import INCLUDE_DIRS, CLIB_DEBUG_PATH, CBIN_DEBUG_PATH, CLIB_RELEASE_PATH, CBIN_RELEASE_PATH, CLIB_DLL_NAMES, CLIB_SO_NAMES, SHLIB_PATH


def _build_shared_lib() -> bool:
    """Build the underlying C library"""
    # TODO: Implement building the C library from Python
    return False


# Manually copy the corresponding shared libraries
def _get_shared_lib(flag=False):
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
    else:
        if flag:
            return None
        if not _build_shared_lib():
            return None
        return _get_shared_lib(True)
    return path, files


ret = _get_shared_lib()
if ret is None:
    print("ERROR: Could not find shared library files", file=sys.stderr)
    sys.exit(1)
path, files = ret
for f in files:
    shutil.copyfile(str(Path.joinpath(path, f)),
                    str(Path.joinpath(SHLIB_PATH, f)))


# Parameters for the FFI

LIB_TYPEDEF = """
#define CL_MEM_READ_ONLY ...
#define CL_MEM_WRITE_ONLY ...
#define CL_MEM_READ_WRITE ...

typedef struct UT_hash_bucket {
   struct UT_hash_handle *hh_head;
   unsigned count;
   unsigned expand_mult;
} UT_hash_bucket;

typedef struct UT_hash_table {
   UT_hash_bucket *buckets;
   unsigned num_buckets, log2_num_buckets;
   unsigned num_items;
   struct UT_hash_handle *tail;
   ptrdiff_t hho;
   unsigned ideal_chain_maxlen;
   unsigned nonideal_items;
   unsigned ineff_expands, noexpand;
   uint32_t signature;
} UT_hash_table;

typedef struct UT_hash_handle {
   struct UT_hash_table *tbl;
   void *prev;
   void *next;
   struct UT_hash_handle *hh_prev;
   struct UT_hash_handle *hh_next;
   const void *key;
   unsigned keylen;
   unsigned hashv;
} UT_hash_handle;

typedef char *va_list;

typedef signed char cl_char;
typedef unsigned char cl_uchar;
typedef signed short cl_short;
typedef unsigned short cl_ushort;
typedef signed int cl_int;
typedef unsigned int cl_uint;
typedef signed long long cl_long;
typedef unsigned long long cl_ulong;
typedef unsigned short cl_half;
typedef float cl_float;
typedef double cl_double;

typedef cl_ulong cl_bitfield;
typedef cl_bitfield cl_mem_flags;

typedef struct _cl_device_type *cl_device_type;
typedef struct _cl_mem *cl_mem;
typedef struct _cl_kernel *cl_kernel;
typedef struct _cl_program *cl_program;
typedef struct _cl_context_properties *cl_context_properties;
typedef struct _cl_queue_properties *cl_queue_properties;
typedef struct _cl_context *cl_context;
typedef struct _cl_command_queue *cl_command_queue;
typedef struct _cl_platform_id *cl_platform_id;
typedef struct _cl_device_id *cl_device_id;
"""

FFI_CDEF = f"""
// Typedefs
{LIB_TYPEDEF}
{_status.CTYPEDEF}
{_devices.CTYPEDEF}
{_cl._kernel.CTYPEDEF}
{_cl._program.CTYPEDEF}
{_cl._runtime.CTYPEDEF}
{_buffer.CTYPEDEF}
// Function definitions
{_status.CDEF}
{_devices.CDEF}
{_cl._kernel.CDEF}
{_cl._program.CDEF}
{_cl._runtime.CDEF}
{_buffer.CDEF}
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
    library_dirs=[str(CBIN_DEBUG_PATH), str(CLIB_DEBUG_PATH),
                  str(CBIN_RELEASE_PATH), str(CLIB_RELEASE_PATH)],
    include_dirs=INCLUDE_DIRS,
    # extra_link_args=["-Wl,-rpath,."],
)
