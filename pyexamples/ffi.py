from pathlib import Path
import os
from otto.defs import ROOT_PATH

os.add_dll_directory(str(Path.joinpath(ROOT_PATH, "pysrc")))
import _otto


def main():
    pass
