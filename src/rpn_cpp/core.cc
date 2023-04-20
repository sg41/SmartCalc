#include "core.h"

#include <stack>
#include <stdexcept>

double CalcEngine::rpn_reduce(double x) {
  std::stack<ExprToken> k;
  double a, b;
  double op;
  for (auto i : rpn_expr_) {
    if (i->state() == OPERAND) {
      k.emplace(i->state(), i->data());
    } else if (i->state() == VARIABLE) {
      k.emplace(i->state(), x);
    } else {
      if (i->state() == OPERATOR) {
        b = k.top().data();
        k.pop();
      }
      a = k.top().data();
      k.pop();
      op = i->data();
      switch ((char)op) {
        case '+':
          k.emplace(OPERAND, a + b);
          break;
        case '-':
          k.emplace(OPERAND, a - b);
          break;
        case '*':
          k.emplace(OPERAND, a * b);
          break;
        case '/':
          k.emplace(OPERAND, (b != 0) ? a / b : NAN);
          break;
        case '%':
          k.emplace(OPERAND, fmod(a, b));
          break;
        case '^':
          k.emplace(OPERAND, pow(a, b));
          break;
        case UNARY_MINUS:
          k.emplace(OPERAND, -a);
          break;
        case UNARY_PLUS:
          k.emplace(OPERAND, a);
          break;
        case sin_FUNCTION:
          k.emplace(OPERAND, sin(a));
          break;
        case cos_FUNCTION:
          k.emplace(OPERAND, cos(a));
          break;
        case tan_FUNCTION:
          k.emplace(OPERAND, tan(a));
          break;
        case sqrt_FUNCTION:
          k.emplace(OPERAND, sqrt(a));
          break;
        case log_FUNCTION:
          k.emplace(OPERAND, log10(a));
          break;
        case ln_FUNCTION:
          k.emplace(OPERAND, log(a));
          break;
        case asin_FUNCTION:
          k.emplace(OPERAND, asin(a));
          break;
        case acos_FUNCTION:
          k.emplace(OPERAND, acos(a));
          break;
        case atan_FUNCTION:
          k.emplace(OPERAND, atan(a));
          break;
        case abs_FUNCTION:
          k.emplace(OPERAND, fabs(a));
          break;
        case ERROR:
        default:
          throw std::invalid_argument("Wrong expression token");
          break;
      }
    }
  }
  double res = k.top().data();
  return res;
}
