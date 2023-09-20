/*
This example is copied over from
https://www.eriksmistad.no/getting-started-with-opencl-and-gpu-computing/
*/

#include "CL/cl.h"
#include <stdio.h>
#include <stdlib.h>

#include <ottou/log.h>
#include <ottou/macros.h>

#include <otto/cl/cl.h>
#include <otto/cl/kernel.h>
#include <otto/cl/program.h>
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
  const int LIST_SIZE = 256;
  OTTO_CALL(otto_vector_with_capacity(LIST_SIZE, sizeof(int), &a),
            "Failed allocating A");
  OTTO_CALL(otto_vector_with_capacity(LIST_SIZE, sizeof(int), &b),
            "Failed allocating B");
  OTTO_CALL(otto_vector_with_capacity(LIST_SIZE, sizeof(int), &out),
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
  otto_kernelht_t *ht = NULL;
  OTTO_CALL(otto_runtime_new(NULL, NULL, OTTO_DEVICE_GPU, ht, &ctx),
            "Could not initialize the runtime");
  cl_int err;

  log_info("Creating the buffers in device memory");
  OTTO_CALL(otto_vector_register(&a, &ctx, CL_MEM_READ_ONLY),
            "Failed registering A");
  OTTO_CALL(otto_vector_register(&b, &ctx, CL_MEM_READ_ONLY),
            "Failed registering B");
  OTTO_CALL(otto_vector_register(&out, &ctx, CL_MEM_WRITE_ONLY),
            "Failed registering OUT");

  log_info("Creating the program");
  otto_program_t prog;
  OTTO_CALL(
      otto_program_from_sources(&ctx, (const char **)&source_str, 1, "", &prog),
      "Failed to create the program");

  log_info("Creating the kernel");
  OTTO_CALL(otto_kernel_new(&prog, "otto_vector_add", 3, &ctx, NULL),
            "Failed creating kernel");

  otto_kernel_t kernel;
  OTTO_CALL(otto_runtime_get_kernel(&ctx, "otto_vector_add", &kernel),
            "Failed getting the kernel");

  log_info("Setting kernel arguments");
  OTTO_CALL(otto_kernel_vcall(&kernel, sizeof(a.gmem), &a.gmem, sizeof(b.gmem),
                              &b.gmem, sizeof(out.gmem), &out.gmem),
            "Failed calling kernel");

  log_info("Executing kernel");
  size_t global_item_size = out.capacity; // Process the entire lists
  size_t local_item_size = 64;            // Divide work items into groups of 64
  OTTO_CL_CALL(clEnqueueNDRangeKernel(ctx.cq, kernel.k, 1, NULL,
                                      &global_item_size, &local_item_size, 0,
                                      NULL, NULL),
               "Could not run kernel");

  log_info("Reading from the output buffer");
  OTTO_CL_CALL(clEnqueueReadBuffer(ctx.cq, out.gmem, CL_TRUE, 0,
                                   out.capacity * out.data_size, out.data, 0,
                                   NULL, NULL),
               "Failed reading from OUT");

  // TODO: Replace this hacky solution for the proper one
  out.len = out.capacity;

  log_info("Displaying results");
  for (int i = 0; i < LIST_SIZE; i++) {
    int a_v, b_v, out_v;
    OTTO_CALL(otto_vector_get(&a, i, &a_v), "Could not get A[%d]", i);
    OTTO_CALL(otto_vector_get(&b, i, &b_v), "Could not get B[%d]", i);
    OTTO_CALL(otto_vector_get(&out, i, &out_v), "Could not get OUT[%d]", i);
    printf("%d + %d = %d\n", a_v, b_v, out_v);
  }

  log_info("Doing final cleanup");
  OTTO_CALL(otto_vector_cleanup(&a), "Failed cleaning A");
  OTTO_CALL(otto_vector_cleanup(&b), "Failed cleaning B");
  OTTO_CALL(otto_vector_cleanup(&out), "Failed cleaning OUT");
  OTTO_CALL(otto_runtime_cleanup(&ctx), "Failed cleaning runtime");
  OTTO_CALL(otto_program_cleanup(&prog), "Failed cleaning program");
  return 0;
}
