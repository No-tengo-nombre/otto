from otto.cl.runtime import Runtime, Kernels


def main():
    a = Runtime(kernels=Kernels.ALL)
    b = Runtime()
