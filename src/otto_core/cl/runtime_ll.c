#include <stdlib.h>

#include <otto_utils/macros.h>
#include <otto_utils/vendor/log.h>

#include <otto/cl/runtime.h>
#include <otto/status.h>

otto_status_t otto_kernelll_push(otto_kernelll_t *head, otto_kernel_t *val) {
  // We can assume head is never NULL, and we assume val is already
  // heap allocated
  if (head->kernel == NULL) {
    // This should only happen if it is the first element
    head->kernel = val;
    return OTTO_STATUS_SUCCESS;
  }

  // Go to the end
  while (head->next != NULL) {
    head = head->next;
  }
  otto_kernelll_t *node = malloc(sizeof(otto_kernelll_t));
  if (node == NULL) {
    logi_error("Could not allocate memory");
    return OTTO_STATUS_FAILURE;
  }
  node->kernel = val;
  node->next = NULL;
  head->next = node;
  return OTTO_STATUS_SUCCESS;
}

otto_status_t otto_kernelll_cleanup(otto_kernelll_t *head) {
  if (head == NULL) {
    return OTTO_STATUS_SUCCESS;
  }
  otto_kernelll_cleanup(head->next); // This never fails
  free(head->kernel);
  free(head);
  return OTTO_STATUS_SUCCESS;
}
