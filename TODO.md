# TODO
This file contains a list of the TODO tasks found within the source code.
- **(`./CMakeLists.txt`)** Implement option to build the Python wrapper along with the C lib
- **(`./cffi_build.py`)** Implement building the C library from Python
- **(`./dependencies/OpenCL/include/CL/opencl.hpp`)** This should really have a D3D10 rerror code!
- **(`./include/otto/cl/runtime.h`)** Add the option to load multiple kernel extensions
- **(`./include/otto/cl/runtime.h`)** Include support for user-created kernel extensions
- **(`./pysrc/otto_ffi/__init__.py`)** Update the FFI to account for the interface changes
- **(`./pysrc/otto/vector.py`)** Fix the bug that happens when the first element is less general than the rest
- **(`./pysrc/otto/vector.py`)** Determine a more efficient way of creating from numpy array
- **(`./pysrc/otto/vector.py`)** Determine a more efficient way of converting to numpy array
- **(`./pysrc/otto/vector.py`)** Implement arithmetic operations using OpenCL kernels
- **(`./pysrc/otto/cl/runtime.py`)** Add option to specify context and queue properties
- **(`./pysrc/otto/cl/runtime.py`)** Figure out why it fails if using CPU (likely an otto error)
- **(`./pysrc/otto/cl/runtime.py`)** Implement the rest of the methods
- **(`./pysrc/otto/__init__.py`)** Wrap the entire library
- **(`./tools/gen_todo.sh`)** Ignore TODOs in dependencies
