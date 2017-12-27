#include "openclutils.h"

namespace  clutils
{

CLConfiguration clConfig;

const char *getErrorString(cl_int errcode)
{
    switch(errcode)
    {
    // run-time and JIT compiler errors
    case 0: return "CL_SUCCESS";
    case -1: return "CL_DEVICE_NOT_FOUND";
    case -2: return "CL_DEVICE_NOT_AVAILABLE";
    case -3: return "CL_COMPILER_NOT_AVAILABLE";
    case -4: return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
    case -5: return "CL_OUT_OF_RESOURCES";
    case -6: return "CL_OUT_OF_HOST_MEMORY";
    case -7: return "CL_PROFILING_INFO_NOT_AVAILABLE";
    case -8: return "CL_MEM_COPY_OVERLAP";
    case -9: return "CL_IMAGE_FORMAT_MISMATCH";
    case -10: return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
    case -11: return "CL_BUILD_PROGRAM_FAILURE";
    case -12: return "CL_MAP_FAILURE";
    case -13: return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
    case -14: return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
    case -15: return "CL_COMPILE_PROGRAM_FAILURE";
    case -16: return "CL_LINKER_NOT_AVAILABLE";
    case -17: return "CL_LINK_PROGRAM_FAILURE";
    case -18: return "CL_DEVICE_PARTITION_FAILED";
    case -19: return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";

    // compile-time errors
    case -30: return "CL_INVALID_VALUE";
    case -31: return "CL_INVALID_DEVICE_TYPE";
    case -32: return "CL_INVALID_PLATFORM";
    case -33: return "CL_INVALID_DEVICE";
    case -34: return "CL_INVALID_CONTEXT";
    case -35: return "CL_INVALID_QUEUE_PROPERTIES";
    case -36: return "CL_INVALID_COMMAND_QUEUE";
    case -37: return "CL_INVALID_HOST_PTR";
    case -38: return "CL_INVALID_MEM_OBJECT";
    case -39: return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
    case -40: return "CL_INVALID_IMAGE_SIZE";
    case -41: return "CL_INVALID_SAMPLER";
    case -42: return "CL_INVALID_BINARY";
    case -43: return "CL_INVALID_BUILD_OPTIONS";
    case -44: return "CL_INVALID_PROGRAM";
    case -45: return "CL_INVALID_PROGRAM_EXECUTABLE";
    case -46: return "CL_INVALID_KERNEL_NAME";
    case -47: return "CL_INVALID_KERNEL_DEFINITION";
    case -48: return "CL_INVALID_KERNEL";
    case -49: return "CL_INVALID_ARG_INDEX";
    case -50: return "CL_INVALID_ARG_VALUE";
    case -51: return "CL_INVALID_ARG_SIZE";
    case -52: return "CL_INVALID_KERNEL_ARGS";
    case -53: return "CL_INVALID_WORK_DIMENSION";
    case -54: return "CL_INVALID_WORK_GROUP_SIZE";
    case -55: return "CL_INVALID_WORK_ITEM_SIZE";
    case -56: return "CL_INVALID_GLOBAL_OFFSET";
    case -57: return "CL_INVALID_EVENT_WAIT_LIST";
    case -58: return "CL_INVALID_EVENT";
    case -59: return "CL_INVALID_OPERATION";
    case -60: return "CL_INVALID_GL_OBJECT";
    case -61: return "CL_INVALID_BUFFER_SIZE";
    case -62: return "CL_INVALID_MIP_LEVEL";
    case -63: return "CL_INVALID_GLOBAL_WORK_SIZE";
    case -64: return "CL_INVALID_PROPERTY";
    case -65: return "CL_INVALID_IMAGE_DESCRIPTOR";
    case -66: return "CL_INVALID_COMPILER_OPTIONS";
    case -67: return "CL_INVALID_LINKER_OPTIONS";
    case -68: return "CL_INVALID_DEVICE_PARTITION_COUNT";

    // extension errors
    case -1000: return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
    case -1001: return "CL_PLATFORM_NOT_FOUND_KHR";
    case -1002: return "CL_INVALID_D3D10_DEVICE_KHR";
    case -1003: return "CL_INVALID_D3D10_RESOURCE_KHR";
    case -1004: return "CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR";
    case -1005: return "CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR";
    default: return "Unknown OpenCL error";
    }
}

void contextReportError(const char* errstr, const void*, size_t, void*)
{
    if (errstr != NULL)
    {
        std::cout << "Error creating OpenCL context: " << errstr << std::endl;
    }
}

bool initOpenCL(cl_context& con, cl_device_id& dev_id, cl_platform_id& plat)
{
    cl_int err_code;
    cl_platform_id platforms[10];
    cl_uint platformsAvailable;
    clGetPlatformIDs(10, platforms, &platformsAvailable);
    if (platformsAvailable == 0)
    {
        std::cout << "Did not get any platforms" << std::endl;
        return false;
    }

    for (uInt i = 0; i < platformsAvailable; i++)
    {
        std::cout << "Platform " << i+1 << " / " << platformsAvailable << std::endl;
        char platform_profile[2048];
        char platform_version[2048];
        char platform_name[2048];
        char platform_vendor[2048];
        char platform_extensions[2048];

        size_t profile_str_size, version_str_size;
        err_code = clGetPlatformInfo(platforms[i], CL_PLATFORM_PROFILE, 2048, platform_profile, &profile_str_size);
        CHECK_ERROR_RET(err_code, "clGetPlatformInfo profile");
        err_code = clGetPlatformInfo(platforms[i], CL_PLATFORM_VERSION, 2048, platform_version, &version_str_size);
        CHECK_ERROR_RET(err_code, "clGetPlatformInfo version");
        err_code = clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, 2048, platform_name, &version_str_size);
        CHECK_ERROR_RET(err_code, "clGetPlatformInfo name");
        err_code = clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, 2048, platform_vendor, &version_str_size);
        CHECK_ERROR_RET(err_code, "clGetPlatformInfo vendor");
        err_code = clGetPlatformInfo(platforms[i], CL_PLATFORM_EXTENSIONS, 2048, platform_extensions, &version_str_size);
        CHECK_ERROR_RET(err_code, "clGetPlatformInfo extensions");

        std::cout << "\tPlatform profile: " << platform_profile << std::endl;
        std::cout << "\tPlatform version: " << platform_version << std::endl;
        std::cout << "\tPlatform name: " << platform_name << std::endl;
        std::cout << "\tPlatform vendor: " << platform_vendor << std::endl;
        std::cout << "\tPlatform extensions: " << platform_extensions << std::endl;
    }

    cl_device_id devices[10];
    cl_uint devices_found;
    err_code = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_GPU, 10, devices, &devices_found);
    CHECK_ERROR_RET(err_code, "clGetDeviceIDs");

    for (uInt i = 0; i < devices_found; i++)
    {
        std::cout << "device " << i+1 << " / " << devices_found << std::endl;

        cl_device_type devtype;
        cl_uint vendor_id;
        cl_uint max_compute_units;
        cl_platform_id device_platform[1024];
        cl_bool compiler;
        char device_name[1024];
        char device_vendor[1024];
        char device_driver[512];
        char device_profile[512];
        char device_versions[512];
        char device_opencl_c_version[1024];
        char device_extensions[8196];

        err_code = clGetDeviceInfo(devices[i], CL_DEVICE_TYPE, sizeof(devtype), &devtype, NULL);
        CHECK_ERROR_RET(err_code, "clGetDeviceInfo type");
        err_code = clGetDeviceInfo(devices[i], CL_DEVICE_VENDOR_ID, sizeof(vendor_id), &vendor_id, NULL);
        CHECK_ERROR_RET(err_code, "clGetDeviceInfo vendor id");
        err_code = clGetDeviceInfo(devices[i], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(max_compute_units), &max_compute_units, NULL);
        CHECK_ERROR_RET(err_code, "clGetDeviceInfo CL_DEVICE_MAX_COMPUTE_UNITS");
        err_code = clGetDeviceInfo(devices[i], CL_DEVICE_PLATFORM, sizeof(cl_platform_id), &device_platform, NULL);
        CHECK_ERROR_RET(err_code, "clGetDeviceInfo CL_DEVICE_PLATFORM");
        err_code = clGetDeviceInfo(devices[i], CL_DEVICE_COMPILER_AVAILABLE, sizeof(cl_bool), &compiler, NULL);
        CHECK_ERROR_RET(err_code, "clGetDeviceInfo CL_DEVICE_COMPILER_AVAILABLE");
        err_code = clGetDeviceInfo(devices[i], CL_DEVICE_NAME, 1024, &device_name, NULL);
        CHECK_ERROR_RET(err_code, "clGetDeviceInfo CL_DEVICE_NAME");
        err_code = clGetDeviceInfo(devices[i], CL_DEVICE_VENDOR, 1024, &device_vendor, NULL);
        CHECK_ERROR_RET(err_code, "clGetDeviceInfo CL_DEVICE_VENDOR");
        err_code = clGetDeviceInfo(devices[i], CL_DRIVER_VERSION, 1024, &device_driver, NULL);
        CHECK_ERROR_RET(err_code, "clGetDeviceInfo CL_DRIVER_VERSION");
        err_code = clGetDeviceInfo(devices[i], CL_DEVICE_PROFILE, 1024, &device_profile, NULL);
        CHECK_ERROR_RET(err_code, "clGetDeviceInfo CL_DEVICE_PROFILE");
        err_code = clGetDeviceInfo(devices[i], CL_DEVICE_VERSION, 1024, &device_versions, NULL);
        CHECK_ERROR_RET(err_code, "clGetDeviceInfo CL_DEVICE_VERSION");
        err_code = clGetDeviceInfo(devices[i], CL_DEVICE_OPENCL_C_VERSION, 1024, &device_opencl_c_version, NULL);
        CHECK_ERROR_RET(err_code, "clGetDeviceInfo CL_DEVICE_OPENCL_C_VERSION");
        err_code = clGetDeviceInfo(devices[i], CL_DEVICE_EXTENSIONS, 1024, &device_extensions, NULL);
        CHECK_ERROR_RET(err_code, "clGetDeviceInfo CL_DEVICE_EXTENSIONS");
        err_code = clGetDeviceInfo(devices[i], CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &clConfig.MAX_LOCAL_WORK_SIZE, NULL);
        CHECK_ERROR_RET(err_code, "clGetDeviceInfo CL_DEVICE_MAX_WORK_GROUP_SIZE");
        err_code = clGetDeviceInfo(devices[i], CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(cl_uint), &clConfig.MAX_WORK_ITEM_DIMENSIONS, NULL);

        if (clConfig.MAX_WORK_ITEM_DIMENSIONS >= 100)
        {
            std::cout << "Too many work item dimensions: " << clConfig.MAX_WORK_ITEM_DIMENSIONS << std::endl;
            return 1;
        }

        err_code = clGetDeviceInfo(devices[i], CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(size_t)*clConfig.MAX_WORK_ITEM_DIMENSIONS, &clConfig.MAX_WORK_ITEM_SIZES, NULL);
        CHECK_ERROR_RET(err_code, "clGetDeviceInfo CL_DEVICE_MAX_WORK_ITEM_SIZES");

        std::cout << "\ttdevtype: " << devtype << std::endl;
        std::cout << "\tvendor_id: " << vendor_id << std::endl;
        std::cout << "\tmax_compute_units: " << max_compute_units << std::endl;
        std::cout << "\tdevice_platform: " << device_platform << std::endl;
        std::cout << "\tcompiler: " << compiler << std::endl;
        std::cout << "\tdevice_name: " << device_name << std::endl;
        std::cout << "\tdevice_vendor: " << device_vendor << std::endl;
        std::cout << "\tdevice_driver: " << device_driver << std::endl;
        std::cout << "\tdevice_profile: " << device_profile << std::endl;
        std::cout << "\tdevice_versions: " << device_versions << std::endl;
        std::cout << "\tdevice_opencl_c_version: " << device_opencl_c_version << std::endl;
        std::cout << "\tdevice_extensions: " << device_extensions << std::endl;
        std::cout << "\tdevice_max_work_group_size: " << clConfig.MAX_LOCAL_WORK_SIZE << std::endl;

        for (uInt i = 0; i < clConfig.MAX_WORK_ITEM_DIMENSIONS; i++)
        {
            std::cout << "\t\tMax work item dimension " << i << ": " << clConfig.MAX_WORK_ITEM_SIZES[i] << std::endl;
        }
    }

    cl_context_properties cprops[3] = { CL_CONTEXT_PLATFORM, (cl_context_properties)(platforms[0]), 0};
    cl_context context = clCreateContext(cprops, 1, devices, &contextReportError, NULL, &err_code);
    CHECK_ERROR_RET(err_code, "clCreateContext");
    con = context;
    plat = platforms[0];
    dev_id = devices[0];

    return true;
}

bool releaseProgram(OpenCLProgram& program)
{
    std::cout << "Releasing OpenCL program " << program._filepath.c_str() << std::endl;
    cl_int err_code = clReleaseProgram(program._program);
    CHECK_ERROR_RET(err_code, "clReleaseProgram");
    return true;
}

bool compileProgram(cl_context& context, const cl_device_id& dev_id, OpenCLProgram& program)
{
    cl_int err_code;
    std::ifstream file(program._filepath);

    if (file.is_open() == false)
    {
        std::cout << "Failed open source: " << program._filepath.c_str() << std::endl;
        return false;
    }

    std::string source(std::istreambuf_iterator<char>(file), (std::istreambuf_iterator<char>()));
    const char* strings[1];
    strings[0] = source.c_str();
    cl_program prog = clCreateProgramWithSource(context, 1, strings, NULL, &err_code);
    CHECK_ERROR_RET(err_code, "clCreateProgramWithSource");

    cl_device_id devices[1];
    devices[0] = dev_id;
    err_code = clBuildProgram(prog, 1, devices, NULL, NULL, NULL);
    if (err_code != CL_BUILD_SUCCESS)
    {
        std::cout << "Error building OpenCL program: " << program._filepath.c_str() << std::endl;
        char build_log[8196];
        err_code = clGetProgramBuildInfo(prog, dev_id, CL_PROGRAM_BUILD_LOG, 8196, build_log, NULL);
        CHECK_ERROR_RET(err_code, "clGetProgramBuildInfo build log");
        std::cout << "Build log:" << std::endl;
        std::cout << build_log << std::endl;
        return false;
    }

    program._program = prog;

    cl_kernel kernels[100];

    cl_uint kernels_ret;
    err_code = clCreateKernelsInProgram(prog, 100, kernels, &kernels_ret);
    CHECK_ERROR(err_code, "clCreateKernel");
    if (err_code != CL_SUCCESS)
    {
        releaseProgram(program);
        return false;
    }

    char function_name[1024];

    for (unsigned int i = 0; i < kernels_ret; i++)
    {
        err_code = clGetKernelInfo(kernels[i], CL_KERNEL_FUNCTION_NAME, 1024, function_name, NULL);
        if (err_code != CL_SUCCESS)
        {
            std::cout << "Failed getting kernel info: " << function_name << ", " << err_code << std::endl;
            releaseProgram(program);
            return false;
        }
        std::cout << "Appended kernel " << i+1 << " / " << kernels_ret << ": " << function_name << std::endl;
        Kernel kern;
        kern._kernel = kernels[i];
        kern._name = function_name;
        program._kernels[function_name] = kern;
        size_t max_size = 0;
        err_code = clGetKernelWorkGroupInfo(kernels[i], dev_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(size_t), &max_size, NULL);
        if (err_code != CL_SUCCESS)
        {
            std::cout << "failed querying kernel max group size: " << function_name << ", " << err_code << std::endl,
            releaseProgram(program);
            return false;
        }
        program._kernels[function_name]._maxWorkGroupSize = max_size;
    }

    return true;
}

bool allocateBuffer(cl_context& context, OpenCLBuffer& buf, cl_mem_flags memflags, void* hostbuffer, size_t size)
{
    cl_int err_code;
    buf._buffer = clCreateBuffer(context, memflags, size, hostbuffer, &err_code);
    buf._memflags = memflags;
    buf._hostbuf = hostbuffer;
    CHECK_ERROR_RET(err_code, "clCreateBuffer");
    return true;
}

bool releaseBuffer(OpenCLBuffer& buf)
{
    cl_int err_code = clReleaseMemObject(buf._buffer);
    CHECK_ERROR_RET(err_code, "clReleaseMemObject");
    return true;
}

bool readBuffer(cl_command_queue& queue, OpenCLBuffer buf, void* dest, size_t size)
{
    cl_int err_code = clEnqueueReadBuffer(queue, buf._buffer, true, 0, size, dest, 0, NULL, NULL);
    CHECK_ERROR_RET(err_code, "clEnqueueReadBuffer");
    return true;
}

bool createCommandQueue(cl_context& context, cl_device_id& dev, cl_command_queue& queue, bool out_of_order)
{
    cl_int err_code;
    cl_command_queue_properties prop = 0;
    if (out_of_order)
        prop |= CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE;
    queue = clCreateCommandQueue(context, dev, prop, &err_code);
    CHECK_ERROR_RET(err_code, "createCommandQueue");
    return true;
}

bool releaseCommandQueue(cl_command_queue& queue)
{
    cl_int err_code = clReleaseCommandQueue(queue);
    CHECK_ERROR_RET(err_code, "clReleaseCommandQueue");
    return true;
}

bool setArgument(cl_kernel& kernel, int index, size_t arg_size, void* value)
{
    cl_int err_code = clSetKernelArg(kernel, index, arg_size, value);
    CHECK_ERROR_RET(err_code, "clSetKernelArg");
    return true;
}

bool run1DKernel(cl_command_queue& command_queue, Kernel& kernel, uInt x, size_t element_size)
{
    size_t total_size = element_size * x;
    size_t global_work_size[1] = { total_size };
    size_t local_work_size[1] = { total_size };

    if (total_size > kernel._maxWorkGroupSize)
        local_work_size[0] = SMALLER(kernel._maxWorkGroupSize, clConfig.MAX_WORK_ITEM_SIZES[0]);

#ifdef UNIFORM_WORK_GROUP_SIZES
    local_work_size[0] = findDivisor(global_work_size[0], local_work_size[0]);
#endif

    cl_event wait_event;
    cl_int err_code = clEnqueueNDRangeKernel(command_queue, kernel._kernel, 1, NULL, global_work_size, local_work_size, 0, NULL, &wait_event);
    CHECK_ERROR_RET(err_code, "clEnqueueNDRangeKernel 1D");

    cl_event events_to_wait[1] = { wait_event };
    err_code = clWaitForEvents(1, events_to_wait);
    CHECK_ERROR_RET(err_code, "run1DKernel wait event");
    return true;
}

bool run2DKernel(cl_command_queue& command_queue, Kernel& kernel, uInt x, uInt y, size_t element_size)
{
    size_t total_size = x * y * element_size;
    size_t global_work_size[2] = { x * element_size, y };
    size_t local_work_size[2] = { x * element_size, y };

    if (total_size > kernel._maxWorkGroupSize)
    {
        local_work_size[0] = SMALLER((uInt)floor(sqrt(kernel._maxWorkGroupSize)), clConfig.MAX_WORK_ITEM_SIZES[0]);
        local_work_size[1] = SMALLER(local_work_size[0], clConfig.MAX_WORK_ITEM_SIZES[1]);
    }

#ifdef UNIFORM_WORK_GROUP_SIZES
    for (int dim = 0; dim < 2; dim++)
        local_work_size[dim] = findDivisor(global_work_size[dim], local_work_size[dim]);
#endif

    cl_event wait_event;
    cl_int err_code = clEnqueueNDRangeKernel(command_queue, kernel._kernel, 2, NULL, global_work_size, local_work_size, 0, NULL, &wait_event);
    CHECK_ERROR_RET(err_code, "clEnqueueNDRangeKernel 2D");

    cl_event events_to_wait[1] = { wait_event };
    err_code = clWaitForEvents(1, events_to_wait);
    CHECK_ERROR_RET(err_code, "run2DKernel wait event");
    return true;
}

bool run3DKernel(cl_command_queue& command_queue, Kernel& kernel, uInt x, uInt y, uInt z, size_t element_size)
{
    size_t total_size = x * y * z * element_size;
    size_t global_work_size[3] = { x * element_size, y, z };
    size_t local_work_size[3] = { x * element_size, y, z };

    if (total_size > kernel._maxWorkGroupSize)
    {
        local_work_size[0] = SMALLER((uInt)floor(cbrt(kernel._maxWorkGroupSize)), clConfig.MAX_WORK_ITEM_SIZES[0]);
        local_work_size[1] = SMALLER(local_work_size[0], clConfig.MAX_WORK_ITEM_SIZES[1]);
        local_work_size[2] = SMALLER(local_work_size[0], clConfig.MAX_WORK_ITEM_SIZES[2]);
    }

#ifdef UNIFORM_WORK_GROUP_SIZES
    for (int dim = 0; dim < 3; dim++)
        local_work_size[dim] = findDivisor(global_work_size[dim], local_work_size[dim]);
#endif

    cl_event wait_event;
    cl_int err_code = clEnqueueNDRangeKernel(command_queue, kernel._kernel, 3, NULL, global_work_size, local_work_size, 0, NULL, &wait_event);
    CHECK_ERROR_RET(err_code, "clEnqueueNDRangeKernel");

    cl_event events_to_wait[1] = { wait_event };
    err_code = clWaitForEvents(1, events_to_wait);
    CHECK_ERROR_RET(err_code, "run3DKernel wait event");
    return true;
}

uInt findDivisor(const uInt& global_size, const uInt& maxLocalSize)
{
    uInt current_max = maxLocalSize;
    while (true)
    {
        if (global_size % current_max == 0)
            return current_max;
        if (current_max == 1)
            return current_max;
        current_max--;
    }

    return 1;
}

} // namespace clutils
