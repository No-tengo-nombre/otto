from otto import Vector, dtypes


def main():
    CAP = 10
    a = Vector.zero(CAP, dtypes.INT32)
    print(a)
    print(f"a[2] = {a[2]}")
    print(f"Before modification : {list(a)}")
    a[1] = 15
    print(f"After modification  : {list(a)}")
