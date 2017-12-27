#ifndef OPENCLUTILS_H
#define OPENCLUTILS_H

#include <CL/cl.h>
#include <stdint.h>
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <math.h>

typedef int8_t Int8;
typedef uint8_t uInt8;
typedef int16_t Int16;
typedef uint16_t uInt16;
typedef int32_t Int32;
typedef uint32_t uInt32;
typedef int64_t Int64;
typedef uint64_t uInt64;
typedef uint32_t uInt;
typedef int32_t Int;

// If defined, any global work group sizes must be divisable by local work groups sizes
// ie. a global work group of [1050, 1115] would not be divisable by [32, 32]
// but it would be by [30, 5]
#define UNIFORM_WORK_GROUP_SIZES
#define SMALLER(a, b) a < b ? a : b;

#define CHECK_ERROR(err, func) if (err != CL_SUCCESS) { \
    std::cout << "OpenCL error at file " __FILE__ << " line " << __LINE__ << ": " << err << ", " << getErrorString(err) << ", in call: " << func << std::endl; }
#define CHECK_ERROR_RET(err, func) if (err != CL_SUCCESS) { \
    std::cout << "OpenCL error at file " << __FILE__ << " <line " << __LINE__ << ": " << err << ", " << getErrorString(err) << ", in call: " << func << std::endl; return false; }

namespace clutils
{
typedef struct
{
    size_t MAX_LOCAL_WORK_SIZE = 0;
    size_t MAX_WORK_ITEM_SIZES[100] = { 0 };
    uInt MAX_WORK_ITEM_DIMENSIONS = 0;
    uInt MAX_KERNEL_WORK_GROUP_SIZE = 0;
} CLConfiguration;

typedef struct
{
    cl_mem _buffer;
    cl_mem_flags _memflags;
    void* _hostbuf;
} OpenCLBuffer;

typedef struct
{
    cl_kernel _kernel;
    uInt _maxWorkGroupSize;
    std::string _name;
} Kernel;

/**
  * Container for an OpenCL program
  *
  * Construct a new OpenCL program by calling createProgram(), making sure to have set _filepath and _entrypoint first
  * Delete an OpenCL program by calling releaseProgram() with a valid _program
  */
typedef struct
{
    cl_program _program;
    std::unordered_map<std::string, Kernel> _kernels;
    std::string _entrypoint;
    std::string _filepath;
    std::unordered_map<std::string, OpenCLBuffer> _buffers;
    void* _host_buffer;
    cl_mem_flags _memflags;
} OpenCLProgram;

extern CLConfiguration clConfig;

const char* getErrorString(cl_int errcode);
void contextReportError(const char* errstr, const void*, size_t, void*);
bool initOpenCL(cl_context& con, cl_device_id& dev_id, cl_platform_id& plat);
bool releaseProgram(OpenCLProgram& program);
bool compileProgram(cl_context& context, const cl_device_id& dev_id, OpenCLProgram& program);
bool allocateBuffer(cl_context& context, OpenCLBuffer& buf, cl_mem_flags memflags, void* hostbuffer, size_t size);
bool releaseBuffer(OpenCLBuffer& buf);
bool readBuffer(cl_command_queue& queue, OpenCLBuffer buf, void* dest, size_t size);
bool createCommandQueue(cl_context& context, cl_device_id& dev, cl_command_queue& queue, bool out_of_order);
bool releaseCommandQueue(cl_command_queue& queue);
bool setArgument(cl_kernel& kernel, int index, size_t arg_size, void* value);
bool run1DKernel(cl_command_queue& command_queue, Kernel& kernel, uInt x, size_t element_size);
bool run2DKernel(cl_command_queue& command_queue, Kernel& kernel, uInt x, uInt y, size_t element_size);
bool run3DKernel(cl_command_queue& command_queue, Kernel& kernel, uInt x, uInt y, uInt z, size_t element_size);
uInt findDivisor(const uInt& global_size, const uInt& maxLocalSize);

} // namespace clutils

#endif // OPENCLUTILS_H
