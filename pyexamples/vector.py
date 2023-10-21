from otto import Vector, dtypes


def main():
    a = Vector.zero(10, dtypes.INT32)
    print(a)
    print(a.capacity)
