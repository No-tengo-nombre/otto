#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <uthash.h>

#include <otto_utils/macros.h>
#include <otto_utils/vendor/log.h>

#include <otto/cl/cl.h>
#include <otto/cl/kernel.h>
#include <otto/cl/runtime.h>
#include <otto/devices.h>
#include <otto/status.h>

otto_status_t otto_runtime_new(const cl_context_properties *ctx_props,
                               const cl_queue_properties *q_props,
                               const otto_device_t type,
                               otto_kernelht_t *kernel_ht,
                               otto_runtime_t *out) {
  if (out == NULL) {
    return OTTO_STATUS_FAILURE;
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
    return OTTO_STATUS_FAILURE;
  }

  logi_info("Creating command queue");
  cl_command_queue cq =
      clCreateCommandQueueWithProperties(ctx, devices, q_props, &status);
  if (cq == NULL || status != CL_SUCCESS) {
    logi_error("Failed creating command queue");
    return OTTO_STATUS_FAILURE;
  }

  logi_debug("Allocating memory for the linked list");
  otto_kernelll_t *node = malloc(sizeof(otto_kernelll_t));
  if (node == NULL) {
    logi_error("Could not allocate memory for linked list");
    return OTTO_STATUS_FAILURE;
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
  };
  *out = result;
  return OTTO_STATUS_SUCCESS;
}

otto_status_t otto_runtime_cleanup(const otto_runtime_t *ctx) {
  otto_kernelll_cleanup(ctx->_kernels_ll);
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
    return OTTO_STATUS_FAILURE;
  }
  logi_debug("Finding '%s' in hashtable", name);
  HASH_FIND_STR(ctx->_kernels_ht, name, item);
  logi_debug("Copying data to output");
  if (item == NULL) {
    logi_error("Could not find kernel");
    return OTTO_STATUS_FAILURE;
  }
  *out = *item->kernel;
  logi_debug("Finished copying data");
  return OTTO_STATUS_SUCCESS;
}

otto_status_t otto_runtime_vcall_kernel(const otto_runtime_t *ctx,
                                        const char *name,
                                        const otto_kernel_args_t *hparams,
                                        ...) {
  // TODO: Defer this functionality to the `otto_kernel_vcall` function
  if (ctx == NULL) {
    logi_error("Runtime is NULL");
    return OTTO_STATUS_FAILURE;
  }

  otto_kernel_t ker;
  OTTO_CALL_I(otto_runtime_get_kernel(ctx, name, &ker),
              "Could not get the kernel");

  logi_info("Calling kernel '%s'", ker.name);

  va_list args;
  va_start(args, hparams);

  for (uint32_t i = 0; i < ker.nargs; i++) {
    size_t size = va_arg(args, size_t);
    void *arg = va_arg(args, void *);
    OTTO_CL_CALL_I(clSetKernelArg(ker.k, i, size, arg),
                   "Failed passing %d-th arg to the kernel", i);
  }

  otto_status_t s = OTTO_STATUS_SUCCESS;
  // Try to call with passed hparams first
  if (hparams != NULL) {
    logi_debug("Calling with given hparams");
    s = clEnqueueNDRangeKernel(ctx->cq, ker.k, hparams->work_dim, NULL,
                               &hparams->global_size, &hparams->local_size, 0,
                               NULL, NULL);
    if (s != OTTO_STATUS_SUCCESS) {
      logi_warn("Calling with given hparams failed");
    }
  }

  // Call with derived hparams if none were passed or calling with passed failed
  if ((hparams == NULL || s == OTTO_STATUS_FAILURE) &&
      ctx->kernel_hparams != NULL) {
    logi_debug("Defaulting to derived hparams");
    s = clEnqueueNDRangeKernel(ctx->cq, ker.k, ctx->kernel_hparams->work_dim,
                               NULL, &ctx->kernel_hparams->global_size,
                               &ctx->kernel_hparams->local_size, 0, NULL, NULL);
    if (s != OTTO_STATUS_SUCCESS) {
      logi_error("Calling with derived hparams failed");
      return OTTO_STATUS_FAILURE;
    }
  }

  // Fail if no hparams were found
  if (hparams == NULL && ctx->kernel_hparams == NULL) {
    logi_error("No hyperparameters were found. Either pass them as an argument "
               "to use them locally, or keep them in the `kernel_hparams` "
               "attribute in the runtime to use them globally");
    return OTTO_STATUS_FAILURE;
  }

  return OTTO_STATUS_SUCCESS;
}
