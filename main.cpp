#include <iostream>
#include <CL/cl.h>

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

#define CHECK_ERROR(err, func) if (err != CL_SUCCESS) { \
    std::cout << "OpenCL error at line " << __LINE__ << ": " << err << ", " << getErrorString(err) << ", in call: " << func << std::endl; }
#define CHECK_ERROR_RET(err, func) if (err != CL_SUCCESS) { \
    std::cout << "OpenCL error at line " << __LINE__ << ": " << err << ", " << getErrorString(err) << ", in call: " << func << std::endl; return false; }

size_t MAX_LOCAL_WORK_SIZE = 0;
size_t MAX_WORK_ITEM_SIZES[100] = { 0 } ;
uInt MAX_WORK_ITEM_DIMENSIONS = 0;
uInt MAX_KERNEL_WORK_GROUP_SIZE = 0;

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
        err_code = clGetDeviceInfo(devices[i], CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &MAX_LOCAL_WORK_SIZE, NULL);
        CHECK_ERROR_RET(err_code, "clGetDeviceInfo CL_DEVICE_MAX_WORK_GROUP_SIZE");
        err_code = clGetDeviceInfo(devices[i], CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(cl_uint), &MAX_WORK_ITEM_DIMENSIONS, NULL);

        if (MAX_WORK_ITEM_DIMENSIONS >= 100)
        {
            std::cout << "Too many work item dimensions: " << MAX_WORK_ITEM_DIMENSIONS << std::endl;
            return 1;
        }

        err_code = clGetDeviceInfo(devices[i], CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(size_t)*MAX_WORK_ITEM_DIMENSIONS, &MAX_WORK_ITEM_SIZES, NULL);
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
        std::cout << "\tdevice_max_work_group_size: " << MAX_LOCAL_WORK_SIZE << std::endl;
        for (uInt i = 0; i < MAX_WORK_ITEM_DIMENSIONS; i++)
        {
            std::cout << "\t\tMax work item dimension " << i << ": " << MAX_WORK_ITEM_SIZES[i] << std::endl;
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

int main(int argc, char *argv[])
{
    cl_context context;
    cl_device_id device_id;
    cl_platform_id platform_id;
    if (!initOpenCL(context, device_id, platform_id))
        return 1;

    return 0;
}

