from otto import Vector, dtypes


def main():
    CAP = 10
    a = Vector.zero(CAP, dtypes.INT32)
    print(a)
    print(f"a[2] = {a[2]}")
    print(f"Before modification : {a.to_list()}")
    a[1] = 15
    print(f"After modification  : {a.to_list()}")

    a.push(1)
    a.push(3)
    a.push(8)
    print(f"After pushes        : {a.to_list()}")

    a.extend_from_list([6, 1, 8, 2])
    print(f"After extension     : {a.to_list()}")
    print(a)

    b = Vector.from_list([1, 25, 2, 111, -1])
    print(f"'b' is {b.to_list()}")
    print(b)
