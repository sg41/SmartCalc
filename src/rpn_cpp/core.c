#include "core.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

double rpn_reduce(const struct expr *e, double x) {
  struct stk *k = stk_new();
  double a, b;
  double op;
  for (struct ll_node *i = e->head; i != NULL; i = i->next) {
    if (i->state == OPERAND) {
      stk_push(k, i->state, i->datum);
    } else if (i->state == VARIABLE) {
      stk_push(k, i->state, x);
    } else {
      if (i->state == OPERATOR) {
        b = stk_peek(k);
        stk_pop(k);
      }
      a = stk_peek(k);
      stk_pop(k);
      op = i->datum;
      switch ((char)op) {
        case '+':
          stk_push(k, OPERAND, a + b);
          break;
        case '-':
          stk_push(k, OPERAND, a - b);
          break;
        case '*':
          stk_push(k, OPERAND, a * b);
          break;
        case '/':
          stk_push(k, OPERAND, (b != 0) ? a / b : NAN);
          break;
        case '%':
          stk_push(k, OPERAND, fmod(a, b));
          break;
        case '^':
          stk_push(k, OPERAND, pow(a, b));
          break;
        case UNARY_MINUS:
          stk_push(k, OPERAND, -a);
          break;
        case UNARY_PLUS:
          stk_push(k, OPERAND, a);
          break;
        case sin_FUNCTION:
          stk_push(k, OPERAND, sin(a));
          break;
        case cos_FUNCTION:
          stk_push(k, OPERAND, cos(a));
          break;
        case tan_FUNCTION:
          stk_push(k, OPERAND, tan(a));
          break;
        case sqrt_FUNCTION:
          stk_push(k, OPERAND, sqrt(a));
          break;
        case log_FUNCTION:
          stk_push(k, OPERAND, log10(a));
          break;
        case ln_FUNCTION:
          stk_push(k, OPERAND, log(a));
          break;
        case asin_FUNCTION:
          stk_push(k, OPERAND, asin(a));
          break;
        case acos_FUNCTION:
          stk_push(k, OPERAND, acos(a));
          break;
        case atan_FUNCTION:
          stk_push(k, OPERAND, atan(a));
          break;
        case abs_FUNCTION:
          stk_push(k, OPERAND, fabs(a));
          break;
        case ERROR:
        default:
          assert(0);
          break;
      }
    }
  }
  double res = stk_peek(k);
  stk_destroy(&k);
  return res;
}

double calc(char *string, float x, int *good) {
  double res = 0;

  if (strlen(string)) {
    struct expr *infix;
    *good = 1;
    infix = expr_from_string(string, good);
    if (*good) {
      struct expr *postfix;
      postfix = expr_shunt(infix);
      res = rpn_reduce(postfix, x);
      expr_destroy(&postfix);
    }
    expr_destroy(&infix);
  }
  return res;
}
