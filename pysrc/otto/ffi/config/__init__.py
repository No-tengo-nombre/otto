from pathlib import Path

from otto.defs import ROOT_PATH


CLIB_DEBUG_DIRNAME = "debug"

CDEPS_PATH = Path.joinpath(ROOT_PATH, "dependencies")
CHEADER_PATH = Path.joinpath(ROOT_PATH, "include")
CLIB_RELEASE_PATH = Path.joinpath(ROOT_PATH, "lib")
CLIB_DEBUG_PATH = Path.joinpath(ROOT_PATH, "lib", CLIB_DEBUG_DIRNAME)
CSOURCES_PATH = Path.joinpath(ROOT_PATH, "src")

INCLUDE_DIRS = [
    str(Path.joinpath(CSOURCES_PATH)),
    str(Path.joinpath(CHEADER_PATH)),
    str(Path.joinpath(CDEPS_PATH, "OpenCL", "include")),
    str(Path.joinpath(CDEPS_PATH, "uthash", "src")),
]
