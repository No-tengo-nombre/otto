import numpy as np

from otto.cl import Runtime, Device
from otto import Vector
from otto_utils.logger import LOGGER


def main(size=256):
    size = int(size)

    LOGGER.info("Setting runtime parameters")
    Runtime.cls_device = Device.GPU

    LOGGER.info("Creating vectors")
    a = Vector.from_numpy(np.arange(size)).to_device()
    b = Vector.from_numpy(size - np.arange(size)).to_device()
    out = Vector.with_capacity(size, a._dtype).set_write().to_device()
