"""FFI for the otto C functions"""


import os
from pathlib import Path


lib_path = str(Path(__file__).parent)
try:
    os.environ["LD_LIBRARY_PATH"] = f"{
        lib_path}: {os.environ["LD_LIBRARY_PATH"]}"
except KeyError:
    os.environ["LD_LIBRARY_PATH"] = lib_path

# Add the dll directory to the trusted path
_WINDOWS_NAME = "nt"
if os.name == _WINDOWS_NAME:
    os.add_dll_directory(os.environ["LD_LIBRARY_PATH"])
