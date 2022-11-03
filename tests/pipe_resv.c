#include <stdio.h>

#define CL_TARGET_OPENCL_VERSION 300
#include <CL/opencl.h>

// Auto-gen
#include "pipe_resv_cl.h"

////////////////////////////////////////////////////////////////////////////////

// Use a static data size for simplicity
//
#define DATA_SIZE (16)


int test_pipe_resv(cl_context context, cl_device_id device_id, cl_command_queue commands) {

    int data[DATA_SIZE];              // original data set given to device
    int results[DATA_SIZE];           // results returned from device
    unsigned int correct;               // number of correct results returned
    size_t global;                      // global domain size for our calculation
    size_t local;                       // local domain size for our calculation


    cl_int err;                            // error code returned from api calls
    cl_program program;                 // compute program
    cl_kernel writer_kernel;                   // compute kernel
    cl_kernel reader_kernel;                   // compute kernel
    
    cl_mem input;                       // device memory used for the input array
    cl_mem output;                      // device memory used for the output array
    
    // Fill our data set with random int values
    //
    int i = 0;
    unsigned int count = DATA_SIZE;
    for(i = 0; i < count; i++) {
        data[i] = rand() % 10000;
        results[i] = 0;
    }

    // Create the compute program from the source buffer
    //
    program = clCreateProgramWithSource(context, 1, (const char **) & KernelSource, NULL, &err);
    if (!program)
    {
        printf("Error: Failed to create compute program!\n");
        return EXIT_FAILURE;
    }

    // Build the program executable
    //
    err = clBuildProgram(program, 0, NULL, "-cl-std=CL3.0", NULL, NULL);
    if (err != CL_SUCCESS)
    {
        size_t len;
        char buffer[2048];

        printf("Error: Failed to build program executable!\n");
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
        printf("%s\n", buffer);
        exit(1);
    }

    // Create the compute kernel in the program we wish to run
    //
    writer_kernel = clCreateKernel(program, "pipe_writer", &err);
    if (!writer_kernel || err != CL_SUCCESS)
    {
        printf("Error: Failed to create writer kernel!(%d)\n", err);
        exit(1);
    }

    reader_kernel = clCreateKernel(program, "pipe_reader", &err);
    if (!reader_kernel || err != CL_SUCCESS)
    {
        printf("Error: Failed to create reader kernel!\n");
        exit(1);
    }

    // Create the input and output arrays in device memory for our calculation
    //
    input = clCreateBuffer(context,  CL_MEM_READ_ONLY,  sizeof(int) * count, NULL, NULL);
    output = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(int) * count, NULL, NULL);
    if (!input || !output)
    {
        printf("Error: Failed to allocate device memory!\n");
        exit(1);
    }    
    
    // Write our data set into the input array in device memory 
    //
    err = clEnqueueWriteBuffer(commands, input, CL_TRUE, 0, sizeof(int) * count, data, 0, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to write to source array!\n");
        exit(1);
    }

    cl_mem pipe = clCreatePipe(context, 0, sizeof(int), count, NULL, NULL);
    if (!pipe) {
        printf("Error: Failed to create pipe\n");
        exit(1);
    }

    // Set the arguments to our compute kernel
    //
    err = 0;
    err |= clSetKernelArg(writer_kernel, 0, sizeof(cl_mem), &input);
    err |= clSetKernelArg(writer_kernel, 1, sizeof(cl_mem), &pipe);

    err |= clSetKernelArg(reader_kernel, 0, sizeof(cl_mem), &output);
    err |= clSetKernelArg(reader_kernel, 1, sizeof(cl_mem), &pipe);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to set kernel arguments! %d\n", err);
        exit(1);
    }

    // Get the maximum work group size for executing the kernel on the device
    //
    err = clGetKernelWorkGroupInfo(reader_kernel, device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to retrieve kernel work group info! %d\n", err);
        exit(1);
    }

    // Execute the kernel over the entire range of our 1d input data set
    // using the maximum number of work group items for this device
    //
    global = count;
    cl_event sync;
    err = clEnqueueNDRangeKernel(commands, writer_kernel, 1, NULL, &global, &local, 0, NULL, &sync);
    if (err)
    {
        printf("Error: Failed to execute kernel!\n");
        return EXIT_FAILURE;
    }
    err = clEnqueueNDRangeKernel(commands, reader_kernel, 1, NULL, &global, &local, 0, NULL, &sync);
    if (err)
    {
        printf("Error: Failed to execute kernel!\n");
        return EXIT_FAILURE;
    }

    // Wait for the command commands to get serviced before reading back results
    //
    clFinish(commands);

    // Read back the results from the device to verify the output
    //
    err = clEnqueueReadBuffer( commands, output, CL_TRUE, 0, sizeof(int) * count, results, 0, NULL, NULL );  
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to read output array! %d\n", err);
        exit(1);
    }
    
    // Validate our results
    //
    correct = 0;
    for(i = 0; i < count; i++)
    {
        if(results[i] == data[i])
            correct++;
    }
    
    // Print a brief summary detailing the results
    //
    printf("Computed '%d/%d' correct values!\n", correct, count);

    clReleaseMemObject(input);
    clReleaseMemObject(output);
    clReleaseProgram(program);
    clReleaseKernel(reader_kernel);
    clReleaseKernel(writer_kernel);
}
