#pragma once

#ifdef OTTO_CL_VERSION
#define CL_TARGET_OPENCL_VERSION OTTO_CL_VERSION
#else
#define CL_TARGET_OPENCL_VERSION 300
#endif

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
