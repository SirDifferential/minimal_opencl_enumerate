#include <iostream>
#include "openclutils.h"
#include <string.h>

int main(int, char**)
{
    cl_context context;
    cl_device_id device_id;
    cl_platform_id platform_id;
    if (!clutils::initOpenCL(context, device_id, platform_id))
        return 1;

    clutils::OpenCLProgram copyprog;
    copyprog._entrypoint = "copybuf";
    copyprog._filepath = "copybuf.cl";

    if (!clutils::compileProgram(context, device_id, copyprog))
        return 1;

    fprintf(stderr, "compiled program\n");

    int mem_alloc_start = 128;
    int mem_alloc_end = 15000; // 15000 * 15000 bytes = 225 MB
    int iterations = 8196;
    int current_alloc = mem_alloc_start;
    int increment = 128;

    while (current_alloc < mem_alloc_end)
    {
        unsigned char* testbuf = new unsigned char[current_alloc * current_alloc];
        memset(testbuf, 0, sizeof(char) * current_alloc * current_alloc);
        unsigned char* outbuf = new unsigned char[current_alloc * current_alloc];

        for (int y = 0; y < current_alloc; y++)
        {
            for (int x = 0; x < current_alloc; x++)
            {
                testbuf[y * current_alloc + x] = (x*y) % 255;
            }
        }

        for (int iter = 0; iter < iterations; iter++)
        {
            fprintf(stderr, "Testing %d x %d buffer, iteration %d / %d\n", current_alloc, current_alloc,
                iter, iterations);

            memset(outbuf, 0, sizeof(char) * current_alloc * current_alloc);

            clutils::OpenCLBuffer buf_in;
            clutils::OpenCLBuffer buf_out;

            if (!clutils::allocateBuffer(context, buf_in, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, testbuf, current_alloc * current_alloc))
                return 1;
            if (!clutils::allocateBuffer(context, buf_out, CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, outbuf, current_alloc * current_alloc))
                return 1;

            copyprog._buffers["copybuf_in"] = buf_in;
            copyprog._buffers["copybuf_out"] = buf_out;

            if (!clutils::setArgument(copyprog._kernels["copybuf"]._kernel, 0, sizeof(cl_mem), &buf_in._buffer))
                return 1;
            if (!clutils::setArgument(copyprog._kernels["copybuf"]._kernel, 1, sizeof(cl_mem), &buf_out._buffer))
                return 1;

            cl_command_queue queue;
            if (!clutils::createCommandQueue(context, device_id, queue, true))
                return 1;

            if (!clutils::run2DKernel(queue, copyprog._kernels["copybuf"], current_alloc, current_alloc, 1))
                return 1;

            if (!clutils::readBuffer(queue, buf_out, outbuf, current_alloc * current_alloc))
                return 1;

            for (int y = 0; y < current_alloc; y++)
            {
                for (int x = 0; x < current_alloc; x++)
                {
                    unsigned char val = outbuf[y * current_alloc + x];
                    if (val != (x*y) % 255)
                    {
                        fprintf(stderr, "Invalid data in outbuffer. Expected %d, got %d\n",
                            (x*y) % 255, (int)val);
                        return 1;
                    }
                }
            }

            if (!clutils::releaseCommandQueue(queue))
                return 1;

            if (!clutils::releaseBuffer(buf_in))
                return 1;

            if (!clutils::releaseBuffer(buf_out))
                return 1;
        }

        delete[] testbuf;
        delete[] outbuf;
        if (current_alloc * current_alloc > 1000000)
            fprintf(stderr, "Tested %d megabytes of memory\n", current_alloc * current_alloc / 1024 / 1024);
        else
            fprintf(stderr, "Tested %d bytes of memory\n", current_alloc * current_alloc);

        current_alloc += increment;
    }
}

