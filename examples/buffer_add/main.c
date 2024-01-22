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
#include <otto/buffer.h>

int main(void) {
  /* Runtime creation */
  log_info("Creating the runtime");
  otto_runtime_t ctx;
  otto_kernelht_t *ht = NULL;
  otto_runtime_new(NULL, NULL, OTTO_DEVICE_GPU, ht, &ctx);
  otto_runtime_load_kernels(&ctx, OTTO_KERNELS_CORE, "");

  /* Create the buffers for the kernel call */
  log_info("Creating the buffers");
  otto_buffer_t a;
  otto_buffer_t b;
  otto_buffer_t out;
  const int LIST_SIZE = 256;
  otto_buffer_with_capacity(LIST_SIZE, sizeof(int), &a);
  otto_buffer_with_capacity(LIST_SIZE, sizeof(int), &b);
  otto_buffer_with_capacity(LIST_SIZE, sizeof(int), &out);
  for (int i = 0; i < LIST_SIZE; i++) {
    otto_buffer_push(&a, &i);
    int v = LIST_SIZE - i;
    otto_buffer_push(&b, &v);
  }
  otto_buffer_setwrite(&out);

  log_info("Creating the buffers in device memory");
  otto_buffer_todevice(&a, &ctx);
  otto_buffer_todevice(&b, &ctx);
  otto_buffer_todevice(&out, &ctx);

  /* Call the kernel */
  log_info("Creating hparams");
  otto_kernel_args_t hparams = {
      .work_dim = 1,
      .global_size = out.capacity,
      .local_size = 64,
  };
  log_info("Calling kernel directly from runtime");
  otto_runtime_call_kernel_binop(&ctx, "otto_buffer_add__i32", &hparams, &a, &b,
                                 &out);
  log_info("Reading from the output buffer");
  otto_buffer_tohost(&out, 0);

  log_info("Displaying results");
  for (int i = 0; i < LIST_SIZE; i++) {
    int a_v, b_v, out_v;
    otto_buffer_get(&a, i, &a_v);
    otto_buffer_get(&b, i, &b_v);
    otto_buffer_get(&out, i, &out_v);
    printf("%d + %d = %d\n", a_v, b_v, out_v);
  }

  /* Clean the program */
  log_info("Doing final cleanup");
  otto_buffer_cleanup(&a);
  otto_buffer_cleanup(&b);
  otto_buffer_cleanup(&out);
  otto_runtime_cleanup(&ctx);
  return 0;
}
