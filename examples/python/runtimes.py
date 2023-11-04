from otto.cl.runtime import Runtime, Kernels, Device


def main():
    a = Runtime(device=Device.GPU)
    b = Runtime()
    if id(a) != id(b):
        print("Something went wrong")
    else:
        print("All good!")
