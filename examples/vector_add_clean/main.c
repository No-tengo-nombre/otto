/*
This example is copied over from
https://www.eriksmistad.no/getting-started-with-opencl-and-gpu-computing/
*/

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
  log_debug("Source size: %d", source_size);
  fclose(fp);

  log_info("Creating the runtime");
  otto_runtime_t ctx;
  otto_kernelht_t *ht = NULL;
  otto_runtime_new(NULL, NULL, OTTO_DEVICE_GPU, ht, &ctx);

  log_info("Creating the buffers in device memory");
  otto_vector_todevice(&a, &ctx, CL_MEM_READ_ONLY);
  otto_vector_todevice(&b, &ctx, CL_MEM_READ_ONLY);
  otto_vector_todevice(&out, &ctx, CL_MEM_WRITE_ONLY);

  log_info("Creating the program");
  otto_program_t prog;
  otto_program_from_sources(&ctx, (const char **)&source_str, 1, "", &prog);

  log_info("Creating the kernel");
  otto_kernel_new(&prog, "otto_vector_add", 3, &ctx, NULL);

  log_info("Creating hparams");
  otto_kernel_args_t hparams = {
      .work_dim = 1,
      .global_size = out.capacity,
      .local_size = 64,
  };

  log_info("Calling kernel directly from runtime");
  otto_runtime_call_kernel(&ctx, "otto_vector_add", &hparams, OTTO_VECTORARG(a),
                           OTTO_VECTORARG(b), OTTO_VECTORARG(out));

  log_info("Reading from the output buffer");
  otto_vector_tohost(&out, 0);

  log_info("Displaying results");
  for (int i = 0; i < LIST_SIZE; i++) {
    int a_v, b_v, out_v;
    otto_vector_get(&a, i, &a_v);
    otto_vector_get(&b, i, &b_v);
    otto_vector_get(&out, i, &out_v);
    printf("%d + %d = %d\n", a_v, b_v, out_v);
  }

  log_info("Doing final cleanup");
  otto_vector_cleanup(&a);
  otto_vector_cleanup(&b);
  otto_vector_cleanup(&out);
  otto_runtime_cleanup(&ctx);
  otto_program_cleanup(&prog);
  return 0;
}
