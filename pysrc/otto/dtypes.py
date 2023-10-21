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
# TODO: Define corresponding C types for the `Vector.get` call


UINT8 = DataType(1, "u8", "UINT8")
UINT16 = DataType(2, "u16", "UINT16")
UINT32 = DataType(4, "u32", "UINT32")
UINT64 = DataType(8, "u64", "UINT64")

INT8 = DataType(1, "i8", "INT8")
INT16 = DataType(2, "i16", "INT16")
INT32 = DataType(4, "i32", "INT32")
INT64 = DataType(8, "i64", "INT64")

FLOAT32 = DataType(4, "f32", "FLOAT32")
FLOAT64 = DataType(8, "f64", "FLOAT64")
