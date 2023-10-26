from typing import Any


class DataType:
    def __init__(self, size_bytes, name, long_name) -> None:
        self.size = size_bytes
        self.name = name
        self.long_name = long_name

    @property
    def size_bits(self) -> int:
        return self.size * 8

    def __str__(self) -> str:
        return self.name

    def __repr__(self) -> str:
        return self.long_name

# TODO: Implement interpreting C types as these data types


UINT8 = DataType(1, "u8", "uint8_t")
UINT16 = DataType(2, "u16", "uint16_t")
UINT32 = DataType(4, "u32", "uint32_t")
UINT64 = DataType(8, "u64", "uint64_t")

INT8 = DataType(1, "i8", "int8_t")
INT16 = DataType(2, "i16", "int16_t")
INT32 = DataType(4, "i32", "int32_t")
INT64 = DataType(8, "i64", "int64_t")

FLOAT32 = DataType(4, "f32", "float")
FLOAT64 = DataType(8, "f64", "double")


def get_ctype(value: Any) -> DataType:
    if isinstance(value, int):
        return INT32
    elif isinstance(value, float):
        return FLOAT64
    else:
        raise TypeError(
            f"Conversion of type '{type(value).__name__}' not implemented"
        )
