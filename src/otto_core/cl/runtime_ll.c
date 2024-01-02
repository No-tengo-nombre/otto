#include <stdlib.h>

#include <otto_utils/macros.h>
#include <otto_utils/vendor/log.h>

#include <otto/cl/runtime.h>
#include <otto/status.h>

otto_status_t otto_kernelll_push(otto_kernelll_t *head, otto_kernelht_t *val) {
  // We can assume head is never NULL, and we assume val is already
  // heap allocated
  if (head->item == NULL) {
    // This should only happen if it is the first element
    logi_debug("Writing first element");
    head->item = val;
    return OTTO_SUCCESS;
  }

  // Go to the end
  logi_debug("Going to end of linked list");
  while (head->next != NULL) {
    head = head->next;
  }
  otto_kernelll_t *node = malloc(sizeof(otto_kernelll_t));
  if (node == NULL) {
    logi_error("Could not allocate memory");
    return OTTO_FAILURE;
  }
  logi_debug("Writing to last node");
  node->item = val;
  node->next = NULL;
  head->next = node;
  logi_debug("Returning");
  return OTTO_SUCCESS;
}

otto_status_t otto_kernelll_cleanup(otto_kernelll_t *head) {
  if (head == NULL) {
    return OTTO_SUCCESS;
  }
  otto_kernelll_cleanup(head->next); // This never fails
  free(head->item->kernel);
  free(head->item); // Frees the hash table entry associated
  free(head);
  return OTTO_SUCCESS;
}
