typedef unsigned char u8_t;
typedef unsigned short u16_t;
typedef unsigned int u32_t;
typedef unsigned long long u64_t;
typedef char i8_t;
typedef short i16_t;
typedef int i32_t;
typedef long long i64_t;
typedef float f32_t;
typedef double f64_t;

/* Addition */

__kernel void otto_vector_add__u8(__global const u8_t *lhs,
                                  __global const u8_t *rhs,
                                  __global u8_t *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] + rhs[i];
}
__kernel void otto_vector_add__u16(__global const u16_t *lhs,
                                   __global const u16_t *rhs,
                                   __global u16_t *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] + rhs[i];
}
__kernel void otto_vector_add__u32(__global const u32_t *lhs,
                                   __global const u32_t *rhs,
                                   __global u32_t *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] + rhs[i];
}
__kernel void otto_vector_add__u64(__global const u64_t *lhs,
                                   __global const u64_t *rhs,
                                   __global u64_t *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] + rhs[i];
}
__kernel void otto_vector_add__i8(__global const i8_t *lhs,
                                  __global const i8_t *rhs,
                                  __global i8_t *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] + rhs[i];
}
__kernel void otto_vector_add__i16(__global const i16_t *lhs,
                                   __global const i16_t *rhs,
                                   __global i16_t *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] + rhs[i];
}
__kernel void otto_vector_add__i32(__global const i32_t *lhs,
                                   __global const i32_t *rhs,
                                   __global i32_t *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] + rhs[i];
}
__kernel void otto_vector_add__i64(__global const i64_t *lhs,
                                   __global const i64_t *rhs,
                                   __global i64_t *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] + rhs[i];
}
__kernel void otto_vector_add__f32(__global const f32_t *lhs,
                                   __global const f32_t *rhs,
                                   __global f32_t *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] + rhs[i];
}
__kernel void otto_vector_add__f64(__global const f64_t *lhs,
                                   __global const f64_t *rhs,
                                   __global f64_t *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] + rhs[i];
}

/* Subtraction */

__kernel void otto_vector_sub__u8(__global const u8_t *lhs,
                                  __global const u8_t *rhs,
                                  __global u8_t *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] - rhs[i];
}
__kernel void otto_vector_sub__u16(__global const u16_t *lhs,
                                   __global const u16_t *rhs,
                                   __global u16_t *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] - rhs[i];
}
__kernel void otto_vector_sub__u32(__global const u32_t *lhs,
                                   __global const u32_t *rhs,
                                   __global u32_t *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] - rhs[i];
}
__kernel void otto_vector_sub__u64(__global const u64_t *lhs,
                                   __global const u64_t *rhs,
                                   __global u64_t *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] - rhs[i];
}
__kernel void otto_vector_sub__i8(__global const i8_t *lhs,
                                  __global const i8_t *rhs,
                                  __global i8_t *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] - rhs[i];
}
__kernel void otto_vector_sub__i16(__global const i16_t *lhs,
                                   __global const i16_t *rhs,
                                   __global i16_t *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] - rhs[i];
}
__kernel void otto_vector_sub__i32(__global const i32_t *lhs,
                                   __global const i32_t *rhs,
                                   __global i32_t *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] - rhs[i];
}
__kernel void otto_vector_sub__i64(__global const i64_t *lhs,
                                   __global const i64_t *rhs,
                                   __global i64_t *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] - rhs[i];
}
__kernel void otto_vector_sub__f32(__global const f32_t *lhs,
                                   __global const f32_t *rhs,
                                   __global f32_t *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] - rhs[i];
}
__kernel void otto_vector_sub__f64(__global const f64_t *lhs,
                                   __global const f64_t *rhs,
                                   __global f64_t *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] - rhs[i];
}

/* Multiplication */

__kernel void otto_vector_mul__u8(__global const u8_t *lhs,
                                  __global const u8_t *rhs,
                                  __global u8_t *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] * rhs[i];
}
__kernel void otto_vector_mul__u16(__global const u16_t *lhs,
                                   __global const u16_t *rhs,
                                   __global u16_t *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] * rhs[i];
}
__kernel void otto_vector_mul__u32(__global const u32_t *lhs,
                                   __global const u32_t *rhs,
                                   __global u32_t *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] * rhs[i];
}
__kernel void otto_vector_mul__u64(__global const u64_t *lhs,
                                   __global const u64_t *rhs,
                                   __global u64_t *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] * rhs[i];
}
__kernel void otto_vector_mul__i8(__global const i8_t *lhs,
                                  __global const i8_t *rhs,
                                  __global i8_t *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] * rhs[i];
}
__kernel void otto_vector_mul__i16(__global const i16_t *lhs,
                                   __global const i16_t *rhs,
                                   __global i16_t *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] * rhs[i];
}
__kernel void otto_vector_mul__i32(__global const i32_t *lhs,
                                   __global const i32_t *rhs,
                                   __global i32_t *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] * rhs[i];
}
__kernel void otto_vector_mul__i64(__global const i64_t *lhs,
                                   __global const i64_t *rhs,
                                   __global i64_t *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] * rhs[i];
}
__kernel void otto_vector_mul__f32(__global const f32_t *lhs,
                                   __global const f32_t *rhs,
                                   __global f32_t *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] * rhs[i];
}
__kernel void otto_vector_mul__f64(__global const f64_t *lhs,
                                   __global const f64_t *rhs,
                                   __global f64_t *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] * rhs[i];
}

/* Division */

__kernel void otto_vector_div__u8(__global const u8_t *lhs,
                                  __global const u8_t *rhs,
                                  __global u8_t *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] / rhs[i];
}
__kernel void otto_vector_div__u16(__global const u16_t *lhs,
                                   __global const u16_t *rhs,
                                   __global u16_t *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] / rhs[i];
}
__kernel void otto_vector_div__u32(__global const u32_t *lhs,
                                   __global const u32_t *rhs,
                                   __global u32_t *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] / rhs[i];
}
__kernel void otto_vector_div__u64(__global const u64_t *lhs,
                                   __global const u64_t *rhs,
                                   __global u64_t *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] / rhs[i];
}
__kernel void otto_vector_div__i8(__global const i8_t *lhs,
                                  __global const i8_t *rhs,
                                  __global i8_t *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] / rhs[i];
}
__kernel void otto_vector_div__i16(__global const i16_t *lhs,
                                   __global const i16_t *rhs,
                                   __global i16_t *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] / rhs[i];
}
__kernel void otto_vector_div__i32(__global const i32_t *lhs,
                                   __global const i32_t *rhs,
                                   __global i32_t *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] / rhs[i];
}
__kernel void otto_vector_div__i64(__global const i64_t *lhs,
                                   __global const i64_t *rhs,
                                   __global i64_t *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] / rhs[i];
}
__kernel void otto_vector_div__f32(__global const f32_t *lhs,
                                   __global const f32_t *rhs,
                                   __global f32_t *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] / rhs[i];
}
__kernel void otto_vector_div__f64(__global const f64_t *lhs,
                                   __global const f64_t *rhs,
                                   __global f64_t *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] / rhs[i];
}
