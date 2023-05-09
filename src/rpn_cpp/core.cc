#include "core.h"

#include <math.h>

#include <stack>
#include <stdexcept>
using namespace s21;
void CalcCore::move_infix_to_rpn(TokenList &infix) {
  if (&infix != &rpn_expr_) {
    std::stack<ExprToken *> opstack;
    TokenList brackets;

    for (auto i : infix) {
      switch (i->state()) {
        case OPERAND:
        case VARIABLE:
          rpn_expr_.push_back(i);
          break;
        case L_BRACKET:
          opstack.push(i);
          brackets.push_back(i);  // store bracket token to be 'delete'd
          break;
        case R_BRACKET:
          while (opstack.top()->state() != L_BRACKET) stack_to_rpn(opstack);
          opstack.pop();  // Забираем из стека открывающуюся скобку
          brackets.push_back(i);  // store bracket token to be 'delete'd
          break;
        case FUNCTION:
        case UNARYOPERATOR:
        case OPERATOR:
          while (opstack.size() > 0 &&
                 opstack.top()->priority() >= i->priority() &&
                 opstack.top()->state() != L_BRACKET)
            stack_to_rpn(opstack);
          opstack.push(i);
          break;
        case ERROR:
        default:
          throw std::invalid_argument("Wrong infix list");
      }
    }
    while (opstack.size() > 0) stack_to_rpn(opstack);
    infix.clear();
  }
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
        if (!k.empty()) {
          b = k.top().data();
          k.pop();
        } else
          throw std::invalid_argument("Wrong operator argument");
      }
      if (!k.empty()) {
        a = k.top().data();
        k.pop();
      } else
        throw std::invalid_argument("Wrong function argument");
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
