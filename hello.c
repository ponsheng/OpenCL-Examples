//
// File:       hello.c
//
// Abstract:   A simple "Hello World" compute example showing basic usage of OpenCL which
//             calculates the mathematical square (X[i] = pow(X[i],2)) for a buffer of
//             floating point values.
//             
//
// Version:    <1.0>
//
// Disclaimer: IMPORTANT:  This Apple software is supplied to you by Apple Inc. ("Apple")
//             in consideration of your agreement to the following terms, and your use,
//             installation, modification or redistribution of this Apple software
//             constitutes acceptance of these terms.  If you do not agree with these
//             terms, please do not use, install, modify or redistribute this Apple
//             software.
//
//             In consideration of your agreement to abide by the following terms, and
//             subject to these terms, Apple grants you a personal, non - exclusive
//             license, under Apple's copyrights in this original Apple software ( the
//             "Apple Software" ), to use, reproduce, modify and redistribute the Apple
//             Software, with or without modifications, in source and / or binary forms;
//             provided that if you redistribute the Apple Software in its entirety and
//             without modifications, you must retain this notice and the following text
//             and disclaimers in all such redistributions of the Apple Software. Neither
//             the name, trademarks, service marks or logos of Apple Inc. may be used to
//             endorse or promote products derived from the Apple Software without specific
//             prior written permission from Apple.  Except as expressly stated in this
//             notice, no other rights or licenses, express or implied, are granted by
//             Apple herein, including but not limited to any patent rights that may be
//             infringed by your derivative works or by other works in which the Apple
//             Software may be incorporated.
//
//             The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO
//             WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED
//             WARRANTIES OF NON - INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A
//             PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION
//             ALONE OR IN COMBINATION WITH YOUR PRODUCTS.
//
//             IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR
//             CONSEQUENTIAL DAMAGES ( INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
//             SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
//             INTERRUPTION ) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, MODIFICATION
//             AND / OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER
//             UNDER THEORY OF CONTRACT, TORT ( INCLUDING NEGLIGENCE ), STRICT LIABILITY OR
//             OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Copyright ( C ) 2008 Apple Inc. All Rights Reserved.
//

////////////////////////////////////////////////////////////////////////////////

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define CL_TARGET_OPENCL_VERSION 300
#include <CL/opencl.h>

#include "tests.h"


////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
    int err;                            // error code returned from api calls
      
    cl_device_id device_id;             // compute device id 
    cl_context context;                 // compute context
    cl_command_queue commands;          // compute command queue
    
    cl_platform_id platform;
    cl_uint platform_num;
    err = clGetPlatformIDs(1, &platform, &platform_num);
    if (CL_SUCCESS != err) {
        printf("Error: Failed to get platforms!\n");
        return EXIT_FAILURE;
    }

    if (platform_num == 0) {
        printf("Error: No platform exists!\n");
        return EXIT_FAILURE;
    }

    printf("There is %u platforms\n", platform_num);


    cl_uint device_num;
    cl_device_type device_type = CL_DEVICE_TYPE_ALL;
    err = clGetDeviceIDs(platform, device_type, 1, &device_id, &device_num);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to get devices! (%d)\n", err);
        return EXIT_FAILURE;
    }

    // Connect to a compute device
    err = clGetDeviceIDs(platform, device_type, 1, &device_id, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to create a device group! (%d)\n", err);
        return EXIT_FAILURE;
    }
  
    // Create a compute context 
    //
    context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
    if (!context)
    {
        printf("Error: Failed to create a compute context!\n");
        return EXIT_FAILURE;
    }

    // Create a command commands
    //
    commands = clCreateCommandQueueWithProperties(context, device_id, NULL, &err);
    if (!commands)
    {
        printf("Error: Failed to create a command commands!\n");
        return EXIT_FAILURE;
    }

    test_square(context, device_id, commands);

    // Shutdown and cleanup
    //
    clReleaseCommandQueue(commands);
    clReleaseContext(context);

    return 0;
}

