/*
This example is copied over from
https://www.eriksmistad.no/getting-started-with-opencl-and-gpu-computing/
*/

#include <stdio.h>
#include <stdlib.h>

#include <ottou/log.h>

#include <otto/cl/cl.h>
#include <otto/cl/runtime.h>
#include <otto/devices.h>
#include <otto/status.h>
#include <otto/vector.h>

#define MAX_SOURCE_SIZE (0x100000)

#define CALL(x, msg, ...)                                                      \
  if (x != OTTO_STATUS_SUCCESS) {                                              \
    log_fatal(msg, ##__VA_ARGS__);                                             \
    return 1;                                                                  \
  }

#define CL_CALL(x, msg, ...)                                                   \
  {                                                                            \
    cl_int err_ = x;                                                           \
    if (err_ != CL_SUCCESS) {                                                  \
      log_fatal(msg, ##__VA_ARGS__);                                           \
      return 1;                                                                \
    }                                                                          \
  }

int main(void) {
  log_info("Creating the parameters");
  otto_vector_t a;
  otto_vector_t b;
  otto_vector_t out;
  const int LIST_SIZE = 256;
  CALL(otto_vector_with_capacity(LIST_SIZE, sizeof(int), &a),
       "Failed allocating A");
  CALL(otto_vector_with_capacity(LIST_SIZE, sizeof(int), &b),
       "Failed allocating B");
  CALL(otto_vector_with_capacity(LIST_SIZE, sizeof(int), &out),
       "Failed allocating OUT");
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
  CALL(otto_runtime_new(NULL, NULL, OTTO_DEVICE_GPU, &ctx),
       "Could not initialize the runtime");
  cl_int err;

  log_info("Creating the buffers in device memory");
  CALL(otto_vector_register(&a, &ctx, CL_MEM_READ_ONLY),
       "Failed registering A");
  CALL(otto_vector_register(&b, &ctx, CL_MEM_READ_ONLY),
       "Failed registering B");
  CALL(otto_vector_register(&out, &ctx, CL_MEM_WRITE_ONLY),
       "Failed registering OUT");

  log_info("Creating the program");
  cl_program program =
      clCreateProgramWithSource(ctx.ctx, 1, (const char **)&source_str,
                                (const size_t *)&source_size, &err);

  log_info("Building the program");
  CL_CALL(clBuildProgram(program, 1, &ctx.devices, NULL, NULL, NULL),
          "Failed building program (%d)", err_);

  log_info("Creating the kernel");
  cl_kernel kernel = clCreateKernel(program, "otto_vector_add", &err);
  if (err != CL_SUCCESS) {
    log_fatal("Could not create kernel (%d)", err);
    return 1;
  }

  log_info("Setting kernel arguments");
  CL_CALL(clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&a.gmem),
          "Failed passing A to the kernel (%d)", err_);
  CL_CALL(clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&b.gmem),
          "Failed passing B to the kernel (%d)", err_);
  CL_CALL(clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&out.gmem),
          "Failed passing OUT to the kernel (%d)", err_);

  log_info("Executing kernel");
  size_t global_item_size = out.capacity; // Process the entire lists
  size_t local_item_size = 64;            // Divide work items into groups of 64
  CL_CALL(clEnqueueNDRangeKernel(ctx.cq, kernel, 1, NULL, &global_item_size,
                                 &local_item_size, 0, NULL, NULL),
          "Could not run kernel (%d)", err_);

  log_info("Reading from the output buffer");
  CL_CALL(clEnqueueReadBuffer(ctx.cq, out.gmem, CL_TRUE, 0,
                              out.capacity * out.data_size, out.data, 0, NULL,
                              NULL),
          "Failed reading from OUT (%d)", err_);

  // TODO: Replace this hacky solution for the proper one
  out.len = out.capacity;

  log_info("Displaying results");
  for (int i = 0; i < LIST_SIZE; i++) {
    int a_v, b_v, out_v;
    CALL(otto_vector_get(&a, i, &a_v), "Could not get A[%d]", i);
    CALL(otto_vector_get(&b, i, &b_v), "Could not get B[%d]", i);
    CALL(otto_vector_get(&out, i, &out_v), "Could not get OUT[%d]", i);
    printf("%d + %d = %d\n", a_v, b_v, out_v);
  }

  log_info("Doing final cleanup");
  CL_CALL(clFlush(ctx.cq), "Failed flushing cq (%d)", err_);
  CL_CALL(clFinish(ctx.cq), "Failed finishing cq (%d)", err_);
  CL_CALL(clReleaseKernel(kernel), "Failed releasing kernel (%d)", err_);
  CL_CALL(clReleaseProgram(program), "Failed releasing program (%d)", err_);
  CALL(otto_vector_cleanup(&a), "Failed cleaning A");
  CALL(otto_vector_cleanup(&b), "Failed cleaning B");
  CALL(otto_vector_cleanup(&out), "Failed cleaning OUT");
  CL_CALL(clReleaseCommandQueue(ctx.cq), "Failed releasing cq (%d)", err_);
  CL_CALL(clReleaseContext(ctx.ctx), "Failed releasing ctx (%d)", err_);
  return 0;
}
