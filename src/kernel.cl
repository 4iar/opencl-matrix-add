__kernel void d_add_matrices(__global float* a, __global float* b, __global float* results, const unsigned int count) {
    int i = get_global_id(0);
    results[i] = a[i] + b[i];
}

