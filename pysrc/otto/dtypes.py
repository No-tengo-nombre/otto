class DataType:
    def __init__(self, size_bytes, name) -> None:
        self.size = size_bytes
        self.name = name

    @property
    def size_bits(self) -> int:
        return self.size * 8


UINT8 = DataType(1, "u8")
UINT16 = DataType(2, "u16")
UINT32 = DataType(4, "u32")
UINT64 = DataType(8, "u64")

INT8 = DataType(1, "i8")
INT16 = DataType(2, "i16")
INT32 = DataType(4, "i32")
INT64 = DataType(8, "i64")

FLOAT8 = DataType(1, "f8")
FLOAT16 = DataType(2, "f16")
FLOAT32 = DataType(4, "f32")
FLOAT64 = DataType(8, "f64")
