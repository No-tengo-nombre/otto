"""Basic definitions for the library"""

from pathlib import Path


BASE_PATH = Path(__file__).parent
ROOT_PATH = BASE_PATH.parent.parent

PYBUILD_DIRNAME = "pybuild"
PYBUILD_PATH = Path.joinpath(ROOT_PATH, PYBUILD_DIRNAME)
