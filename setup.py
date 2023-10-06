from setuptools import Extension, setup
import os

LIB_NAME = "otto"

USE_CYTHON = True
ext = "pyx" if USE_CYTHON else "c"

PYX_FILES = [
    os.path.join("pysrc", "otto_pyx", f"status.{ext}"),
    os.path.join("pysrc", "otto_pyx", f"vector.{ext}"),
]


if __name__ == "__main__":
    extensions = [
        Extension(
            LIB_NAME,
            PYX_FILES,
            include_dirs=[
                os.path.join("include"),
                os.path.join("dependencies", "OpenCL", "include"),
                os.path.join("dependencies", "uthash", "src"),
            ],
            libraries=["otto_core", "otto_utils"],
            library_dirs=[
                os.path.join("dependencies", "OpenCL", "lib"),
                os.path.join("lib"),
                os.path.join("lib", "debug"),
                os.path.join("lib", "release")
            ],
        )
    ]

    if USE_CYTHON:
        from Cython.Build import cythonize
        extensions = cythonize(extensions)

    setup(ext_modules=extensions)
