#include "core.h"

#include <math.h>

#include <stack>
#include <stdexcept>

void CalcCore::stack_to_rpn(std::stack<ExprToken *> &opstack) {
  rpn_expr_.push_back(opstack.top());
  opstack.pop();
}

void CalcCore::expr_shunt(TokenList &infix) {
  std::stack<ExprToken *> opstack;

  for (auto i : infix) {
    switch (i->state()) {
      case OPERAND:
      case VARIABLE:
        rpn_expr_.push_back(i);
        break;
      case L_BRACKET:
        opstack.push(i);
        break;
      case R_BRACKET:
        while (opstack.top()->state() != L_BRACKET) stack_to_rpn(opstack);
        opstack.pop();  // Забираем из стека открывающуюся скобку
        if (opstack.top()->state() == FUNCTION) stack_to_rpn(opstack);
        break;
      case FUNCTION:
      case UNARYOPERATOR:
      case OPERATOR:
        while (opstack.size() > 0 && opstack.top()->priorty() >= i->priorty() &&
               opstack.top()->state() != L_BRACKET)
          stack_to_rpn(opstack);
        opstack.push(i);
        break;
      case ERROR:
      default:
        throw std::invalid_argument("Wrong infix list");
    }
    // if (i->state() != L_BRACKET && i->state() != R_BRACKET) infix.remove(i);
  }
  while (opstack.size() > 0) stack_to_rpn(opstack);
}

double CalcCore::rpn_reduce(double x) {
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
        k.emplace(OPERAND, i->func(a, b));
      else if (i->state() == UNARYOPERATOR || i->state() == FUNCTION)
        k.emplace(OPERAND, i->func(a, 0));
      else
        throw std::invalid_argument("Wrong expression token");
    }
  }
  double res = k.top().data();
  return res;
}
