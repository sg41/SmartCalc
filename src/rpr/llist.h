#ifndef SRC_RPR_LLIST_H_
#define SRC_RPR_LLIST_H_

#include <assert.h>

#include "expr.h"

struct ll_node {
  unsigned int state;
  double datum;
  struct ll_node *next;
};

struct ll_node *ll_node_new(const unsigned int s, const double d);
void ll_node_destroy(struct ll_node **n);
struct ll_node *ll_node_prepend(struct ll_node *n, const unsigned int s,
                                const double d);
void ll_node_append(struct ll_node *n, const unsigned int s, const double d);
int ll_print(const struct ll_node *n);
struct ll_node *ll_remove_node(struct ll_node *n, struct ll_node *root);
void ll_destroy_list(struct ll_node *root);
struct ll_node *ll_last_node(struct ll_node *n);
struct ll_node *ll_before_last_node(struct ll_node *n);

#endif  // SRC_RPR_LLIST_H_
