#include <stdio.h>
#include <stdlib.h>

#include <ottou/log.h>
#include <ottou/macros.h>

#include <otto/cl/cl.h>
#include <otto/cl/kernel.h>
#include <otto/cl/program.h>
#include <otto/cl/runtime.h>
#include <otto/devices.h>
#include <otto/paths.h>
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

  log_info("Creating the runtime");
  otto_runtime_t ctx;
  otto_kernelht_t *ht = NULL;
  OTTO_CALL(otto_runtime_new(NULL, NULL, OTTO_DEVICE_GPU, ht, &ctx),
            "Could not initialize the runtime");

  log_info("Creating the buffers in device memory");
  OTTO_CALL(otto_vector_todevice(&a, &ctx, CL_MEM_READ_ONLY),
            "Failed registering A");
  OTTO_CALL(otto_vector_todevice(&b, &ctx, CL_MEM_READ_ONLY),
            "Failed registering B");
  OTTO_CALL(otto_vector_todevice(&out, &ctx, CL_MEM_WRITE_ONLY),
            "Failed registering OUT");

  log_info("Creating the program");
  const char *files[] = {OTTO_CLKERNEL("vector/elementary.cl")};
  otto_program_t prog;
  OTTO_CALL(otto_program_from_files(&ctx, files, 1, "", &prog),
            "Failed to create the program");

  log_info("Creating the kernel");
  OTTO_CALL(otto_kernel_new(&prog, "otto_vector_add", 3, &ctx, NULL),
            "Failed creating kernel");

  log_info("Creating hparams");
  otto_kernel_args_t hparams = {
      .work_dim = 1,
      .global_size = out.capacity,
      .local_size = 64,
  };

  log_info("Calling kernel directly from runtime");
  OTTO_CALL(otto_runtime_call_kernel_binop(&ctx, "otto_vector_add", &hparams,
                                           &a, &b, &out),
            "Failed calling kernel");

  log_info("Reading from the output buffer");
  OTTO_CALL(otto_vector_tohost(&out, 0), "Failed fetching OUT to host");

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
