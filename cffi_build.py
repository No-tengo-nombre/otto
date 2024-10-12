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
from otto_ffi.config import CLIB_DEBUG_PATH, CBIN_DEBUG_PATH, CLIB_RELEASE_PATH, CBIN_RELEASE_PATH, INCLUDE_DIRS


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

typedef ... va_list;

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

typedef ... cl_device_type;
typedef ... cl_context_properties;
typedef ... cl_queue_properties;
typedef struct _cl_platform_id *    cl_platform_id;
typedef struct _cl_device_id *      cl_device_id;
typedef struct _cl_context *        cl_context;
typedef struct _cl_command_queue *  cl_command_queue;
typedef struct _cl_mem *            cl_mem;
typedef struct _cl_program *        cl_program;
typedef struct _cl_kernel *         cl_kernel;
typedef struct _cl_event *          cl_event;
typedef struct _cl_sampler *        cl_sampler;
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

LIB_SOURCE = r"""
#include <otto/otto.h>
"""

# Builder for the FFI in out-of-line API mode

# for i, line in enumerate(FFI_CDEF.splitlines()):
#     print(f"{i:>4} | {line}")

ffi_builder = cffi.FFI()

ffi_builder.cdef(FFI_CDEF)
ffi_builder.set_source(
    "_otto",
    LIB_SOURCE,
    libraries=["otto_core"],
    library_dirs=[str(CBIN_DEBUG_PATH), str(CLIB_DEBUG_PATH),
                  str(CBIN_RELEASE_PATH), str(CLIB_RELEASE_PATH)],
    include_dirs=INCLUDE_DIRS,
    extra_link_args=["-Wl,-rpath=lib/:lib/debug/:/usr/local/lib"],
)


if __name__ == "__main__":
    ffi_builder.compile("pysrc", verbose=True)
