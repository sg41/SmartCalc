#include "core.h"

#include <math.h>

#include <stack>
#include <stdexcept>
using namespace s21;
void CalcCore::moveInfixToRpn(TokenList &infix) {
  if (&infix != &rpn_expr_) {
    std::stack<ExprToken *> opstack;
    TokenList brackets;

    for (auto i : infix) {
      switch (i->state()) {
        case kOperandToken:
        case kVariableToken:
          rpn_expr_.push_back(i);
          break;
        case kLBracketToken:
          opstack.push(i);
          brackets.push_back(i);  // store bracket token to be 'delete'd
          break;
        case kRBracketToken:
          while (opstack.top()->state() != kLBracketToken)
            moveStackToRpn(opstack);
          opstack.pop();  // Забираем из стека открывающуюся скобку
          brackets.push_back(i);  // store bracket token to be 'delete'd
          break;
        case kFunctionToken:
        case kUnaryOperatorToken:
        case kOperatorToken:
          while (opstack.size() > 0 &&
                 opstack.top()->priority() >= i->priority() &&
                 opstack.top()->state() != kLBracketToken)
            moveStackToRpn(opstack);
          opstack.push(i);
          break;
        case kErrorToken:
        default:
          throw std::invalid_argument("Wrong infix list");
      }
    }
    while (opstack.size() > 0) moveStackToRpn(opstack);
    infix.clear();
  }
}

double CalcCore::rpnCalculate(double x) {
  std::stack<ExprToken> k;
  for (auto i : rpn_expr_) {
    if (i->state() == kOperandToken) {
      k.emplace(i->state(), i->data());
    } else if (i->state() == kVariableToken) {
      k.emplace(i->state(), x);
    } else {
      double a, b = 0;
      if (i->state() == kOperatorToken) {
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
      if (i->state() == kOperatorToken)
        k.emplace(kOperandToken, i->func(a, b));
      else if (i->state() == kUnaryOperatorToken ||
               i->state() == kFunctionToken)
        k.emplace(kOperandToken, i->func(a, 0));
      else
        throw std::invalid_argument("Wrong expression token");
    }
  }
  double res = k.top().data();
  return res;
}
