import os
from pathlib import Path


lib_path = str(Path(__file__).parent)
try:
    os.environ["LD_LIBRARY_PATH"] = f"{lib_path}:{os.environ["LD_LIBRARY_PATH"]}"
except KeyError:
    os.environ["LD_LIBRARY_PATH"] = lib_path
