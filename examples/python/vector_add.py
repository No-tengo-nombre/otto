from otto.cl.runtime import Runtime, Kernels, Device


def main():
    Runtime.cls_device = Device.GPU
    Runtime.cls_kernels = Kernels.CORE
    ctx = Runtime()
