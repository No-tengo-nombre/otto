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

int main(void) {
  /* Runtime creation */
  log_info("Creating the runtime");
  otto_runtime_t ctx;
  otto_kernelht_t *ht = NULL;
  otto_runtime_new(NULL, NULL, OTTO_DEVICE_GPU, ht, &ctx);
  otto_runtime_load_kernels(&ctx, OTTO_KERNELS_CORE, "");

  /* Create the vectors for the kernel call */
  log_info("Creating the vectors");
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

  log_info("Creating the buffers in device memory");
  otto_vector_todevice(&a, &ctx, CL_MEM_READ_ONLY);
  otto_vector_todevice(&b, &ctx, CL_MEM_READ_ONLY);
  otto_vector_todevice(&out, &ctx, CL_MEM_WRITE_ONLY);

  /* Call the kernel */
  log_info("Creating hparams");
  otto_kernel_args_t hparams = {
      .work_dim = 1,
      .global_size = out.capacity,
      .local_size = 64,
  };
  log_info("Calling kernel directly from runtime");
  otto_runtime_call_kernel_binop(&ctx, "otto_vector_add", &hparams, &a, &b,
                                 &out);
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

  /* Clean the program */
  log_info("Doing final cleanup");
  otto_vector_cleanup(&a);
  otto_vector_cleanup(&b);
  otto_vector_cleanup(&out);
  otto_runtime_cleanup(&ctx);
  return 0;
}
