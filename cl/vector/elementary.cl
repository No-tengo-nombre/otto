__kernel void otto_vector_add(__global const int *lhs, __global const int *rhs, __global int *out) {
    int i = get_global_id(0);
    out[i] = lhs[i] + rhs[i];
}

__kernel void otto_vector_sub(__global const int *lhs, __global const int *rhs, __global int *out) {
    int i = get_global_id(0);
    out[i] = lhs[i] - rhs[i];
}

__kernel void otto_vector_mul(__global const int *lhs, __global const int *rhs, __global int *out) {
    int i = get_global_id(0);
    out[i] = lhs[i] * rhs[i];
}

__kernel void otto_vector_div(__global const int *lhs, __global const int *rhs, __global int *out) {
    int i = get_global_id(0);
    out[i] = lhs[i] / rhs[i];
}
