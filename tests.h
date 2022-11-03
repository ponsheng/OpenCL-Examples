#ifndef __TESTS_H__
#define __TESTS_H__

#define CL_TARGET_OPENCL_VERSION 300
#include <CL/opencl.h>

#define TEST(NAME) int test_##NAME(cl_context context, cl_device_id device_id, cl_command_queue commands)

TEST(square);
TEST(pipe);
TEST(pipe_resv);

#endif
