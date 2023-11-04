import numpy as np

from otto.cl import Runtime, Device, KernelArgs
from otto import Vector
from otto_utils.logger import LOGGER


def main(size=256):
    size = int(size)

    LOGGER.info("Setting runtime parameters")
    Runtime.cls_device = Device.GPU
    ctx = Runtime()

    LOGGER.info("Creating vectors")
    a = Vector.from_numpy(np.arange(size)).to_device()
    b = Vector.from_numpy(size - np.arange(size)).to_device()
    print(f"a = {a.content_string()}")
    print(f"b = {b.content_string()}")

    hparams = KernelArgs(1, size, 64)
    out = ctx.call_binop_kernel_no_out("otto_vector_add", a, b, hparams)

    out.to_host()
    print(f"out = {out.content_string()}")
