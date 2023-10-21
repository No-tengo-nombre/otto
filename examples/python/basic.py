from otto import Vector, dtypes


def main():
    CAP = 10
    a = Vector[dtypes.INT32].zero(CAP, dtypes.INT32)
    print(a)
    print(f"a[2] = {a[2]}")
    print(f"Before modification : {list(a)}")
    a[1] = 15
    print(f"After modification  : {list(a)}")

    a.push(1)
    a.push(3)
    a.push(8)
    print(f"After pushes        : {list(a)}")

    a.extend_from_list([6, 1, 8, 2])
    print(f"After extension     : {list(a)}")

    print(a)
