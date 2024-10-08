import numpy as np

from otto.cl import Runtime, Device, KernelArgs
from otto import Buffer
from otto_utils.logger import LOGGER


def main(size=256):
    size = int(size)

    LOGGER.info("Setting runtime parameters")
    Runtime.cls_device = Device.GPU
    Runtime.hparams = KernelArgs(1, size, 64)

    LOGGER.info("Creating buffers")
    a = Buffer.from_numpy(np.arange(size)).to_device()
    b = Buffer.from_numpy(size - np.arange(size)).to_device()
    print(f"a = {a}")
    print(f"b = {b}")

    out = a + b

    out.to_host()
    print(f"out = {out}")
