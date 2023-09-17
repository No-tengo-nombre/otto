/*
This example is copied over from
https://www.eriksmistad.no/getting-started-with-opencl-and-gpu-computing/
*/

#include "CL/cl.h"
#include <stdio.h>
#include <stdlib.h>

#include <ottou/log.h>

#include <otto/cl/cl.h>
#include <otto/cl/runtime.h>
#include <otto/devices.h>
#include <otto/status.h>
#include <otto/vector.h>

#define MAX_SOURCE_SIZE (0x100000)

int main(void) {
  log_info("Creating the parameters");
  otto_vector_t a;
  otto_vector_t b;
  otto_vector_t out;
  const int LIST_SIZE = 32;
  otto_vector_with_capacity(LIST_SIZE, sizeof(int), &a);
  otto_vector_with_capacity(LIST_SIZE, sizeof(int), &b);
  otto_vector_with_capacity(LIST_SIZE, sizeof(int), &out);
  for (int i = 0; i < LIST_SIZE; i++) {
    otto_vector_push(&a, &i);
    int v = LIST_SIZE - i;
    otto_vector_push(&b, &v);
  }

  log_info("Loading the kernel");
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

  log_info("Creating the runtime");
  otto_runtime_t ctx;
  if (otto_runtime_new(NULL, NULL, OTTO_DEVICE_GPU, &ctx) ==
      OTTO_STATUS_FAILURE) {
    log_fatal("Could not initialize the runtime");
    return 1;
  }
  cl_int err;

  log_info("Creating the buffers in device memory");
  otto_vector_register(&a, CL_MEM_READ_ONLY, &ctx);
  otto_vector_register(&b, CL_MEM_READ_ONLY, &ctx);
  otto_vector_register(&out, CL_MEM_WRITE_ONLY, &ctx);

  // Create memory buffers on the device for each vector
  // cl_mem a_mem_obj = clCreateBuffer(ctx.ctx, CL_MEM_READ_ONLY,
  //                                   LIST_SIZE * sizeof(int), NULL, &err);
  // cl_mem b_mem_obj = clCreateBuffer(ctx.ctx, CL_MEM_READ_ONLY,
  //                                   LIST_SIZE * sizeof(int), NULL, &err);
  // cl_mem c_mem_obj = clCreateBuffer(ctx.ctx, CL_MEM_WRITE_ONLY,
  //                                   LIST_SIZE * sizeof(int), NULL, &err);

  // log_info("Writing to the buffers");
  // err = clEnqueueWriteBuffer(ctx.cq, a_mem_obj, CL_TRUE, 0,
  //                            LIST_SIZE * sizeof(int), a.data, 0, NULL, NULL);
  // err = clEnqueueWriteBuffer(ctx.cq, b_mem_obj, CL_TRUE, 0,
  //                            LIST_SIZE * sizeof(int), b.data, 0, NULL, NULL);

  log_info("Creating the program");
  cl_program program =
      clCreateProgramWithSource(ctx.ctx, 1, (const char **)&source_str,
                                (const size_t *)&source_size, &err);

  log_info("Building the program");
  err = clBuildProgram(program, 1, &ctx.devices, NULL, NULL, NULL);

  log_info("Creating the kernel");
  cl_kernel kernel = clCreateKernel(program, "otto_vector_add", &err);

  log_info("Setting kernel arguments");
  err = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a.gmem);
  err = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b.gmem);
  err = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&out.gmem);

  log_info("Executing kernel");
  size_t global_item_size = LIST_SIZE; // Process the entire lists
  size_t local_item_size = 64;         // Divide work items into groups of 64
  err = clEnqueueNDRangeKernel(ctx.cq, kernel, 1, NULL, &global_item_size,
                               &local_item_size, 0, NULL, NULL);

  log_info("Reading from the output buffer");
  err = clEnqueueReadBuffer(ctx.cq, out.gmem, CL_TRUE, 0,
                            out.len * out.data_size, out.data, 0, NULL, NULL);

  log_info("Displaying results");
  for (int i = 0; i < LIST_SIZE; i++) {
    int a_v, b_v, out_v;
    otto_vector_get(&a, i, &a_v);
    otto_vector_get(&b, i, &b_v);
    otto_vector_get(&out, i, &out_v);
    printf("%d + %d = %d\n", a_v, b_v, out_v);
  }

  log_info("Doing final cleanup");
  err = clFlush(ctx.cq);
  err = clFinish(ctx.cq);
  err = clReleaseKernel(kernel);
  err = clReleaseProgram(program);
  otto_vector_cleanup(&a);
  otto_vector_cleanup(&b);
  otto_vector_cleanup(&out);
  err = clReleaseCommandQueue(ctx.cq);
  err = clReleaseContext(ctx.ctx);
  return 0;
}
