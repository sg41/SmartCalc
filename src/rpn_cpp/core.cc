#include "core.h"

#include <stack>
#include <stdexcept>

double CalcEngine::rpn_reduce(double x) {
  std::stack<ExprToken> k;
  for (auto i : rpn_expr_) {
    if (i->state() == OPERAND) {
      k.emplace(i->state(), i->data());
    } else if (i->state() == VARIABLE) {
      k.emplace(i->state(), x);
    } else {
      double a, b = 0;
      if (i->state() == OPERATOR) {
        b = k.top().data();
        k.pop();
      }
      a = k.top().data();
      k.pop();
      if (i->state() == OPERATOR)
        k.emplace(OPERAND, i->calc(a, b));
      else
        k.emplace(OPERAND, i->calc(a));
    }
  }
  double res = k.top().data();
  return res;
}
