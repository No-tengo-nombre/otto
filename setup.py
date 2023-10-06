from setuptools import Extension, setup
from setuptools.command.build_ext import build_ext
import os

LIB_NAME = "otto"

USE_CYTHON = True
ext = "pyx" if USE_CYTHON else "c"

PYX_FILES = [
    os.path.join("pysrc", "otto_pyx", f"status.{ext}"),
    os.path.join("pysrc", "otto_pyx", f"vector.{ext}"),
]


link_args = ["-static-libgcc",
             "-static-libstdc++",
             "-Wl,-Bstatic,--whole-archive",
             "-lwinpthread",
             "-Wl,--no-whole-archive"]

class Build(build_ext):
    def build_extensions(self):
        if self.compiler.compiler_type == "mingw64":
            for e in self.extensions:
                e.extra_link_args = link_args
        super(Build, self).build_extensions()


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

    setup(ext_modules=extensions, cmdclass={"build_ext": Build})
