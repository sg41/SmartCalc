#ifndef SRC_RPR_STACK_H_
#define SRC_RPR_STACK_H_
#include "llist.h"

struct stk {
  unsigned int depth;
  struct ll_node *top;
};

struct stk *stk_new(void);
void stk_destroy(struct stk **k);
void stk_push(struct stk *k, const unsigned int s, const double d);
void stk_pop(struct stk *k);
void stk_print(const struct stk *k);
double stk_peek(const struct stk *k);
int stk_peek_status(const struct stk *k);

#endif  // SRC_RPR_STACK_H_
