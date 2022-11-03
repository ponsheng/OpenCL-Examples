#ifndef __TESTS_H__
#define __TESTS_H__

#define CL_TARGET_OPENCL_VERSION 300
#include <CL/opencl.h>

int test_square(cl_context context, cl_device_id device_id, cl_command_queue commands);
int test_pipe(cl_context context, cl_device_id device_id, cl_command_queue commands);

#endif
