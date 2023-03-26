#include "stack.h"

#include <stdio.h>
#include <stdlib.h>

struct stk *stk_new(void) {
  struct stk *k = malloc(sizeof(*k));
  if (k != NULL) {
    k->depth = 0;
    k->top = NULL;
  }
  return k;
}

void stk_destroy(struct stk **k) {
  if (k && *k && (*k)->depth > 0 && (*k)->top != NULL)
    ll_destroy_list((*k)->top);
  free(*k);
}

void stk_push(struct stk *k, const unsigned int s, const double d) {
  if (k != NULL) {
    if (k->depth > 0) {
      k->top = ll_node_prepend(k->top, s, d);
    } else {
      k->top = ll_node_new(s, d);
    }
    (k->depth)++;
  }
}

void stk_pop(struct stk *k) {
  if (k != NULL) {
    if (k->depth > 0) {
      struct ll_node *tmp = k->top->next;
      ll_node_destroy(&(k->top));
      k->top = tmp;
      (k->depth)--;
    } else {
      assert(0);
    }
  }
}

int stk_peek_status(const struct stk *k) {
  return (k != NULL && k->depth > 0) ? k->top->state : 0;
}

double stk_peek(const struct stk *k) {
  return (k != NULL && k->depth > 0) ? k->top->datum : 0;
}
