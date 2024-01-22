#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <uthash.h>

#include <otto_utils/macros.h>
#include <otto_utils/vendor/log.h>

#include <otto/cl/cl.h>
#include <otto/cl/kernel.h>
#include <otto/cl/program.h>
#include <otto/cl/runtime.h>
#include <otto/devices.h>
#include <otto/status.h>
#include <otto/buffer.h>

otto_status_t otto_runtime_new(const cl_context_properties *ctx_props,
                               const cl_queue_properties *q_props,
                               const otto_device_t type,
                               otto_kernelht_t *kernel_ht,
                               otto_runtime_t *out) {
  if (out == NULL) {
    return OTTO_STATUS_FAILURE("out variable can not be NULL");
  }

  cl_device_type dev = CL_DEVICE_TYPE_DEFAULT;
  const char *dev_name;
  OTTO_CALL_I(otto_device_as_cl(type, &dev),
              "Requested device not implemented");
  OTTO_CALL_I(otto_device_name(type, &dev_name),
              "Requested device not implemented");
  logi_info("Using device '%s'", dev_name);

  cl_int status = CL_SUCCESS;

  cl_platform_id platforms = NULL;
  cl_uint platform_num;
  cl_device_id devices = NULL;
  cl_uint device_num;
  logi_info("Getting platforms");
  OTTO_CL_CALL_I(
      clGetPlatformIDs(OTTO_PLATFORM_ENTRIES, &platforms, &platform_num),
      "Failed getting platforms");
  logi_debug("Found %d platforms", platform_num);
  logi_info("Getting devices");
  OTTO_CL_CALL_I(clGetDeviceIDs(platforms, dev, OTTO_DEVICE_ENTRIES, &devices,
                                &device_num),
                 "Failed getting devices");
  logi_debug("Found %d devices", device_num);

  logi_info("Creating context");
  cl_context ctx =
      clCreateContext(ctx_props, device_num, &devices, NULL, NULL, &status);
  if (ctx == NULL || status != CL_SUCCESS) {
    logi_error("Failed creating context");
    return OTTO_STATUS_FAILURE("Failed creating context");
  }

  logi_info("Creating command queue");
  cl_command_queue cq =
      clCreateCommandQueueWithProperties(ctx, devices, q_props, &status);
  if (cq == NULL || status != CL_SUCCESS) {
    logi_error("Failed creating command queue");
    return OTTO_STATUS_FAILURE("Failed creating command queue");
  }

  logi_debug("Allocating memory for the linked list");
  otto_kernelll_t *node = malloc(sizeof(otto_kernelll_t));
  if (node == NULL) {
    logi_error("Could not allocate memory for linked list");
    return OTTO_STATUS_FAILURE("Could not allocate memory for linked list");
  }
  node->item = NULL;
  node->next = NULL;

  logi_info("Assembling struct");
  otto_runtime_t result = {
      .ctx = ctx,
      .cq = cq,
      .platforms = platforms,
      .platform_num = platform_num,
      .devices = devices,
      .device_num = device_num,
      .dev = type,
      ._kernels_ht = kernel_ht,
      ._kernels_ll = node,
      .kernel_hparams = NULL,
      ._sources = NULL,
      ._sources_count = 0,
  };
  *out = result;
  return OTTO_STATUS_SUCCESS;
}

otto_status_t otto_runtime_load_kernels(otto_runtime_t *ctx,
                                        const otto_program_kernels_t kernels,
                                        const char *build_options) {
  otto_program_t prog;
  OTTO_CALL_I(otto_program_from_default(ctx, kernels, build_options, &prog),
              "Could not load program");
  OTTO_CALL_I(otto_program_cleanup(&prog), "Failed cleaning up the program");
  return OTTO_STATUS_SUCCESS;
}

void cleanup_sources(const char **sources, const size_t count) {
  // This should only be called with a malloc'ed array of malloc'ed strings,
  // meaning both sources and all sources[i] are in heap.
  const char **original = sources;
  for (size_t i = 0; i < count; i++, sources++) {
    free((void *)*sources);
  }
  free(original);
}

otto_status_t otto_runtime_cleanup(const otto_runtime_t *ctx) {
  otto_kernelll_cleanup(ctx->_kernels_ll);
  cleanup_sources(ctx->_sources, ctx->_sources_count);
  OTTO_CL_CALL_I(clFlush(ctx->cq), "Failed flushing command queue");
  OTTO_CL_CALL_I(clFinish(ctx->cq), "Failed finishing command queue");
  OTTO_CL_CALL_I(clReleaseCommandQueue(ctx->cq),
                 "Failed releasing command queue");
  OTTO_CL_CALL_I(clReleaseContext(ctx->ctx), "Failed releasing context");
  return OTTO_STATUS_SUCCESS;
}

otto_status_t otto_runtime_add_kernel(otto_runtime_t *ctx, const char *name,
                                      otto_kernel_t *kernel) {
  logi_info("Adding kernel '%s' to the runtime", name);
  logi_debug("Creating hashtable item");
  otto_kernelht_t *item = malloc(sizeof(otto_kernelht_t));
  item->name = name;
  item->kernel = kernel;

  logi_debug("Adding to hashtable");
  HASH_ADD_STR(ctx->_kernels_ht, name, item);
  logi_debug("Pushing to linked list");
  otto_kernelll_push(ctx->_kernels_ll, item);
  return OTTO_STATUS_SUCCESS;
}

otto_status_t otto_runtime_get_kernel(const otto_runtime_t *ctx,
                                      const char *name, otto_kernel_t *out) {
  logi_info("Getting kernel '%s' from the runtime", name);
  otto_kernelht_t *item;

  if (ctx->_kernels_ht == NULL) {
    logi_error("Hash table in the current runtime is NULL");
    return OTTO_STATUS_FAILURE("Hash table in the current runtime is NULL");
  }
  logi_debug("Finding '%s' in hashtable", name);
  HASH_FIND_STR(ctx->_kernels_ht, name, item);
  logi_debug("Copying data to output");
  if (item == NULL) {
    logi_error("Could not find kernel");
    return OTTO_STATUS_FAILURE("Could not find kernel");
  }
  *out = *item->kernel;
  logi_debug("Finished copying data");
  return OTTO_STATUS_SUCCESS;
}

otto_status_t otto_runtime_vcall_kernel(const otto_runtime_t *ctx,
                                        const char *name,
                                        const otto_kernel_args_t *hparams,
                                        va_list args) {
  if (ctx == NULL) {
    logi_error("Runtime is NULL");
    return OTTO_STATUS_FAILURE("Runtime is NULL");
  }

  otto_kernel_t ker;
  OTTO_CALL_I(otto_runtime_get_kernel(ctx, name, &ker),
              "Could not get the kernel");
  return otto_kernel_vcall(&ker, ctx, hparams, args);
}

otto_status_t otto_runtime_call_kernel(const otto_runtime_t *ctx,
                                       const char *name,
                                       const otto_kernel_args_t *hparams, ...) {
  va_list args;
  va_start(args, hparams);
  otto_status_t s = otto_runtime_vcall_kernel(ctx, name, hparams, args);
  va_end(args);
  return s;
}

otto_status_t otto_runtime_call_kernel_binop(const otto_runtime_t *ctx,
                                             const char *name,
                                             const otto_kernel_args_t *hparams,
                                             const otto_buffer_t *a,
                                             const otto_buffer_t *b,
                                             otto_buffer_t *out) {
  return otto_runtime_call_kernel(ctx, name, hparams, OTTO_PVECTORARG(a),
                                  OTTO_PVECTORARG(b), OTTO_PVECTORARG(out));
}
