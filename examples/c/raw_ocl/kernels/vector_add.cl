__kernel void otto_buffer_add(__global const int *a, __global const int *b, __global int *result) {
    int i = get_global_id(0);
    result[i] = a[i] + b[i];
}
