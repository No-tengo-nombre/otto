from pathlib import Path

from otto.defs import ROOT_PATH


CLIB_DEBUG_DIRNAME = "debug"

CHEADER_PATH = Path.joinpath(ROOT_PATH, "include")
CLIB_RELEASE_PATH = Path.joinpath(ROOT_PATH, "lib")
CLIB_DEBUG_PATH = Path.joinpath(ROOT_PATH, "lib", CLIB_DEBUG_DIRNAME)
CSOURCES_PATH = Path.joinpath(ROOT_PATH, "src")
