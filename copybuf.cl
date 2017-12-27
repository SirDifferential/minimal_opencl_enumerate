#pragma OPENCL EXTENSION cl_khr_byte_addressable_store : enable

__kernel void copybuf(__global char* in, __global char* out)
{
    int x = get_global_id(0);
    int y = get_global_id(1);
    const int width = get_global_size(0);
    const int index = y * width + x;
    out[index] = in[index];
}
