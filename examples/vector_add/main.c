/*
This example is copied over from
https://www.eriksmistad.no/getting-started-with-opencl-and-gpu-computing/
*/

#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#include <otto/cl/runtime.h>
#include <otto/status.h>
#include <otto/vector.h>

#define MAX_SOURCE_SIZE (0x100000)

int main(void) {
  // Create the two input vectors
  int i;
  const int LIST_SIZE = 1024;
  int *A = (int *)malloc(sizeof(int) * LIST_SIZE);
  int *B = (int *)malloc(sizeof(int) * LIST_SIZE);
  for (i = 0; i < LIST_SIZE; i++) {
    A[i] = i;
    B[i] = LIST_SIZE - i;
  }

  // Load the kernel source code into the array source_str
  FILE *fp;
  char *source_str;
  size_t source_size;

  fp = fopen("examples/vector_add/ocl/vector_add.cl", "r");
  if (!fp) {
    fprintf(stderr, "Failed to load kernel.\n");
    exit(1);
  }
  source_str = (char *)malloc(MAX_SOURCE_SIZE);
  source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
  fclose(fp);

  otto_runtime_t ctx;
  if (otto_runtime_new(NULL, CL_DEVICE_TYPE_CPU, &ctx) != OTTO_STATUS_FAILURE) {
    return 1;
  }
  cl_int err;

  // Create a command queue
  cl_command_queue command_queue =
      clCreateCommandQueue(ctx.ctx, ctx.devices, 0, &err);

  // Create memory buffers on the device for each vector
  cl_mem a_mem_obj = clCreateBuffer(ctx.ctx, CL_MEM_READ_ONLY,
                                    LIST_SIZE * sizeof(int), NULL, &err);
  cl_mem b_mem_obj = clCreateBuffer(ctx.ctx, CL_MEM_READ_ONLY,
                                    LIST_SIZE * sizeof(int), NULL, &err);
  cl_mem c_mem_obj = clCreateBuffer(ctx.ctx, CL_MEM_WRITE_ONLY,
                                    LIST_SIZE * sizeof(int), NULL, &err);

  // Copy the lists A and B to their respective memory buffers
  err = clEnqueueWriteBuffer(command_queue, a_mem_obj, CL_TRUE, 0,
                             LIST_SIZE * sizeof(int), A, 0, NULL, NULL);
  err = clEnqueueWriteBuffer(command_queue, b_mem_obj, CL_TRUE, 0,
                             LIST_SIZE * sizeof(int), B, 0, NULL, NULL);

  // Create a program from the kernel source
  cl_program program =
      clCreateProgramWithSource(ctx.ctx, 1, (const char **)&source_str,
                                (const size_t *)&source_size, &err);

  // Build the program
  err = clBuildProgram(program, 1, &ctx.devices, NULL, NULL, NULL);

  // Create the OpenCL kernel
  cl_kernel kernel = clCreateKernel(program, "otto_vector_add", &err);

  // Set the arguments of the kernel
  err = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a_mem_obj);
  err = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b_mem_obj);
  err = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&c_mem_obj);

  // Execute the OpenCL kernel on the list
  size_t global_item_size = LIST_SIZE; // Process the entire lists
  size_t local_item_size = 64;         // Divide work items into groups of 64
  err =
      clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size,
                             &local_item_size, 0, NULL, NULL);

  // Read the memory buffer C on the device to the local variable C
  int *C = (int *)malloc(sizeof(int) * LIST_SIZE);
  err = clEnqueueReadBuffer(command_queue, c_mem_obj, CL_TRUE, 0,
                            LIST_SIZE * sizeof(int), C, 0, NULL, NULL);

  // Display the result to the screen
  for (i = 0; i < LIST_SIZE; i++)
    printf("%d + %d = %d\n", A[i], B[i], C[i]);

  // Clean up
  err = clFlush(command_queue);
  err = clFinish(command_queue);
  err = clReleaseKernel(kernel);
  err = clReleaseProgram(program);
  err = clReleaseMemObject(a_mem_obj);
  err = clReleaseMemObject(b_mem_obj);
  err = clReleaseMemObject(c_mem_obj);
  err = clReleaseCommandQueue(command_queue);
  err = clReleaseContext(ctx.ctx);
  free(A);
  free(B);
  free(C);
  return 0;
}
