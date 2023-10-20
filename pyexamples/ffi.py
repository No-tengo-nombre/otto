from pathlib import Path
import os
from otto.defs import ROOT_PATH

# os.add_dll_directory(str(Path.joinpath(ROOT_PATH, "pysrc")))
from _otto import lib as otto


def main():
    a = otto.otto_vector_new(5, None) # This fails but I only care that it works
    print(a)
