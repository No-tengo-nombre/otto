from pathlib import Path


BASE_PATH = Path(__file__).parent.parent
ROOT_PATH = BASE_PATH.parent.parent
SHLIB_PATH = BASE_PATH

PYBUILD_DIRNAME = "pysrc"
PYBUILD_PATH = Path.joinpath(ROOT_PATH, PYBUILD_DIRNAME)

DEBUG_DIRNAME = "debug"

CDEPS_PATH = Path.joinpath(ROOT_PATH, "dependencies")
CHEADER_PATH = Path.joinpath(ROOT_PATH, "include")
CBIN_RELEASE_PATH = Path.joinpath(ROOT_PATH, "bin")
CBIN_DEBUG_PATH = Path.joinpath(ROOT_PATH, "bin", DEBUG_DIRNAME)
CLIB_RELEASE_PATH = Path.joinpath(ROOT_PATH, "lib")
CLIB_DEBUG_PATH = Path.joinpath(ROOT_PATH, "lib", DEBUG_DIRNAME)
CSOURCES_PATH = Path.joinpath(ROOT_PATH, "src")

CLIB_SO_NAMES = [
    "libotto_core.so",
    "libotto_utils.so",
]
CLIB_DLL_NAMES = [
    "otto_core.dll",
    "otto_utils.dll",
]

INCLUDE_DIRS = [
    str(Path.joinpath(CSOURCES_PATH)),
    str(Path.joinpath(CHEADER_PATH)),
    str(Path.joinpath(CDEPS_PATH, "OpenCL", "include")),
    str(Path.joinpath(CDEPS_PATH, "uthash", "src")),
]
