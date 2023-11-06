/* Addition */

__kernel void otto_vector_add__u8(__global const unsigned char *lhs,
                                  __global const unsigned char *rhs,
                                  __global unsigned char *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] + rhs[i];
}
__kernel void otto_vector_add__u16(__global const unsigned short *lhs,
                                   __global const unsigned short *rhs,
                                   __global unsigned short *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] + rhs[i];
}
__kernel void otto_vector_add__u32(__global const unsigned int *lhs,
                                   __global const unsigned int *rhs,
                                   __global unsigned int *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] + rhs[i];
}
__kernel void otto_vector_add__u64(__global const unsigned long long *lhs,
                                   __global const unsigned long long *rhs,
                                   __global unsigned long long *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] + rhs[i];
}
__kernel void otto_vector_add__i8(__global const char *lhs,
                                  __global const char *rhs,
                                  __global char *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] + rhs[i];
}
__kernel void otto_vector_add__i16(__global const short *lhs,
                                   __global const short *rhs,
                                   __global short *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] + rhs[i];
}
__kernel void otto_vector_add__i32(__global const int *lhs,
                                   __global const int *rhs, __global int *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] + rhs[i];
}
__kernel void otto_vector_add__i64(__global const long long *lhs,
                                   __global const long long *rhs,
                                   __global long long *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] + rhs[i];
}
__kernel void otto_vector_add__f32(__global const float *lhs,
                                   __global const float *rhs,
                                   __global float *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] + rhs[i];
}
__kernel void otto_vector_add__f64(__global const double *lhs,
                                   __global const double *rhs,
                                   __global double *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] + rhs[i];
}

/* Subtraction */

__kernel void otto_vector_sub__u8(__global const unsigned char *lhs,
                                  __global const unsigned char *rhs,
                                  __global unsigned char *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] - rhs[i];
}
__kernel void otto_vector_sub__u16(__global const unsigned short *lhs,
                                   __global const unsigned short *rhs,
                                   __global unsigned short *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] - rhs[i];
}
__kernel void otto_vector_sub__u32(__global const unsigned int *lhs,
                                   __global const unsigned int *rhs,
                                   __global unsigned int *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] - rhs[i];
}
__kernel void otto_vector_sub__u64(__global const unsigned long long *lhs,
                                   __global const unsigned long long *rhs,
                                   __global unsigned long long *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] - rhs[i];
}
__kernel void otto_vector_sub__i8(__global const char *lhs,
                                  __global const char *rhs,
                                  __global char *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] - rhs[i];
}
__kernel void otto_vector_sub__i16(__global const short *lhs,
                                   __global const short *rhs,
                                   __global short *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] - rhs[i];
}
__kernel void otto_vector_sub__i32(__global const int *lhs,
                                   __global const int *rhs, __global int *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] - rhs[i];
}
__kernel void otto_vector_sub__i64(__global const long long *lhs,
                                   __global const long long *rhs,
                                   __global long long *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] - rhs[i];
}
__kernel void otto_vector_sub__f32(__global const float *lhs,
                                   __global const float *rhs,
                                   __global float *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] - rhs[i];
}
__kernel void otto_vector_sub__f64(__global const double *lhs,
                                   __global const double *rhs,
                                   __global double *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] - rhs[i];
}

/* Multiplication */

__kernel void otto_vector_mul__u8(__global const unsigned char *lhs,
                                  __global const unsigned char *rhs,
                                  __global unsigned char *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] * rhs[i];
}
__kernel void otto_vector_mul__u16(__global const unsigned short *lhs,
                                   __global const unsigned short *rhs,
                                   __global unsigned short *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] * rhs[i];
}
__kernel void otto_vector_mul__u32(__global const unsigned int *lhs,
                                   __global const unsigned int *rhs,
                                   __global unsigned int *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] * rhs[i];
}
__kernel void otto_vector_mul__u64(__global const unsigned long long *lhs,
                                   __global const unsigned long long *rhs,
                                   __global unsigned long long *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] * rhs[i];
}
__kernel void otto_vector_mul__i8(__global const char *lhs,
                                  __global const char *rhs,
                                  __global char *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] * rhs[i];
}
__kernel void otto_vector_mul__i16(__global const short *lhs,
                                   __global const short *rhs,
                                   __global short *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] * rhs[i];
}
__kernel void otto_vector_mul__i32(__global const int *lhs,
                                   __global const int *rhs, __global int *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] * rhs[i];
}
__kernel void otto_vector_mul__i64(__global const long long *lhs,
                                   __global const long long *rhs,
                                   __global long long *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] * rhs[i];
}
__kernel void otto_vector_mul__f32(__global const float *lhs,
                                   __global const float *rhs,
                                   __global float *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] * rhs[i];
}
__kernel void otto_vector_mul__f64(__global const double *lhs,
                                   __global const double *rhs,
                                   __global double *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] * rhs[i];
}

/* Division */

__kernel void otto_vector_div__u8(__global const unsigned char *lhs,
                                  __global const unsigned char *rhs,
                                  __global unsigned char *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] / rhs[i];
}
__kernel void otto_vector_div__u16(__global const unsigned short *lhs,
                                   __global const unsigned short *rhs,
                                   __global unsigned short *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] / rhs[i];
}
__kernel void otto_vector_div__u32(__global const unsigned int *lhs,
                                   __global const unsigned int *rhs,
                                   __global unsigned int *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] / rhs[i];
}
__kernel void otto_vector_div__u64(__global const unsigned long long *lhs,
                                   __global const unsigned long long *rhs,
                                   __global unsigned long long *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] / rhs[i];
}
__kernel void otto_vector_div__i8(__global const char *lhs,
                                  __global const char *rhs,
                                  __global char *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] / rhs[i];
}
__kernel void otto_vector_div__i16(__global const short *lhs,
                                   __global const short *rhs,
                                   __global short *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] / rhs[i];
}
__kernel void otto_vector_div__i32(__global const int *lhs,
                                   __global const int *rhs, __global int *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] / rhs[i];
}
__kernel void otto_vector_div__i64(__global const long long *lhs,
                                   __global const long long *rhs,
                                   __global long long *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] / rhs[i];
}
__kernel void otto_vector_div__f32(__global const float *lhs,
                                   __global const float *rhs,
                                   __global float *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] / rhs[i];
}
__kernel void otto_vector_div__f64(__global const double *lhs,
                                   __global const double *rhs,
                                   __global double *out) {
  int i = get_global_id(0);
  out[i] = lhs[i] / rhs[i];
}
