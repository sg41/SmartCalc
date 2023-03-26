#include "expr.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

/* Возвращает значение ИСТИНА, если с является оператором. */
int is_oper(char c) {
  int res = 0;
  if (c != 0 && strchr("+-/*%^()", c)) res = 1;
  return res;
}

/* Возвращает значение ИСТИНА, если с является раздилителем, в том числе 0 */
int is_space(char c) { return (strchr(" \t\n\r", c)) ? 1 : 0; }

/*Возвращает значение ИСТИНА, если с является цифрой или
//точкой */
int is_digit(char c) {
  return (c >= '0' && c <= '9') ||
         c == '.';  //|| c == '-' || c == 'e' || c == 'E';
}

/* Возвращает значение ИСТИНА, если с является буквой */
int is_alpha(char c) { return c >= 'A' && c <= 'z'; }

struct expr *expr_new(void) {
  struct expr *e = malloc(sizeof(*e));
  if (e != NULL) {
    e->length = 0;
    e->head = NULL;
  }
  return e;
}

void expr_destroy(struct expr **e) {
  ll_destroy_list((*e)->head);
  free(*e);
}

void expr_add_symbol(struct expr *e, const unsigned int s, const double d) {
  if (e != NULL) {
    if (e->length > 0) {
      ll_node_append(e->head, s, d);
    } else {
      e->head = ll_node_new(s, d);
    }
    (e->length)++;
  }
}

unsigned int precedence(const struct ll_node *n) {
  unsigned int score = 0;
  if (n != NULL) {
    switch ((char)n->datum) {
      case '+':
        score = ADD_SCORE;
        break;
      case '-':
        score = SUB_SCORE;
        break;
      case '*':
        score = MULT_SCORE;
        break;
      case '/':
      case '%':
        score = DIV_SCORE;
        break;
      case '^':
        score = EXP_SCORE;
        break;
      case '(':
        score = L_SCORE;
        break;
      case ')':
        score = R_SCORE;
        break;
      case sin_FUNCTION:
      case cos_FUNCTION:
      case tan_FUNCTION:
      case ctg_FUNCTION:
      case abs_FUNCTION:
      case log_FUNCTION:
      case ln_FUNCTION:
      case sqrt_FUNCTION:
      case asin_FUNCTION:
      case acos_FUNCTION:
      case atan_FUNCTION:
        score = FUNC_SCORE;
        break;
      case UNARY_MINUS:
      case UNARY_PLUS:
        score = U_SCORE;
        break;
      default:
        break;
    }
  }
  return score;
}

void stack_to_expr(struct expr *rpn, struct stk *opstack) {
  expr_add_symbol(rpn, stk_peek_status(opstack), stk_peek(opstack));
  stk_pop(opstack);
}

struct expr *expr_shunt(const struct expr *infix) {
  struct stk *opstack = stk_new();
  struct expr *rpn = expr_new();
  for (struct ll_node *i = infix->head; i != NULL; i = i->next) {
    switch (i->state) {
      case OPERAND:
      case VARIABLE:
        expr_add_symbol(rpn, i->state, i->datum);
        break;
      case L_BRACKET:
        stk_push(opstack, i->state, i->datum);
        break;
      case R_BRACKET:
        while (stk_peek_status(opstack) != L_BRACKET)
          stack_to_expr(rpn, opstack);
        stk_pop(opstack);  // Забираем из стека открывающуюся скобку
        if (stk_peek_status(opstack) == FUNCTION) stack_to_expr(rpn, opstack);
        break;
      case FUNCTION:
      case UNARYOPERATOR:
      case OPERATOR:
        while (opstack->depth > 0 &&
               precedence(opstack->top) >= precedence(i) &&
               stk_peek_status(opstack) != L_BRACKET)
          stack_to_expr(rpn, opstack);
        stk_push(opstack, i->state, i->datum);
        break;
      case ERROR:
      default:
        i = NULL;
        assert(0);
    }
  }
  while (opstack->depth > 0) stack_to_expr(rpn, opstack);
  stk_destroy(&opstack);
  return rpn;
}

double get_operand(const char *a) {
  double d;
  sscanf(a, "%lg", &d);
  return d;
}

char *expr_add_function(struct expr *infix, char *src_str, int *good) {
  if (strncmp(src_str, "sin(", 4) == 0) {
    expr_add_symbol(infix, FUNCTION, sin_FUNCTION);
    src_str += 3;
  } else if (strncmp(src_str, "cos(", 4) == 0) {
    expr_add_symbol(infix, FUNCTION, cos_FUNCTION);
    src_str += 3;
  } else if (strncmp(src_str, "tan(", 4) == 0) {
    expr_add_symbol(infix, FUNCTION, tan_FUNCTION);
    src_str += 3;
  } else if (strncmp(src_str, "abs(", 4) == 0) {
    expr_add_symbol(infix, FUNCTION, abs_FUNCTION);
    src_str += 3;
  } else if (strncmp(src_str, "log(", 4) == 0) {
    expr_add_symbol(infix, FUNCTION, log_FUNCTION);
    src_str += 3;
  } else if (strncmp(src_str, "ln(", 3) == 0) {
    expr_add_symbol(infix, FUNCTION, ln_FUNCTION);
    src_str += 2;
  } else if (strncmp(src_str, "mod", 3) == 0) {
    expr_add_symbol(infix, OPERATOR, mod_FUNCTION);
    src_str += 3;
  } else if (strncmp(src_str, "sqrt(", 5) == 0) {
    expr_add_symbol(infix, FUNCTION, sqrt_FUNCTION);
    src_str += 4;
  } else if (strncmp(src_str, "asin(", 5) == 0) {
    expr_add_symbol(infix, FUNCTION, asin_FUNCTION);
    src_str += 4;
  } else if (strncmp(src_str, "acos(", 5) == 0) {
    expr_add_symbol(infix, FUNCTION, acos_FUNCTION);
    src_str += 4;
  } else if (strncmp(src_str, "atan(", 5) == 0) {
    expr_add_symbol(infix, FUNCTION, atan_FUNCTION);
    src_str += 4;
  } else if (*src_str == 'x' || *src_str == 'X') {
    expr_add_symbol(infix, VARIABLE, 'x');
    src_str++;
  } else {
    while (is_alpha(*src_str)) {
      src_str++;
    }
    expr_add_symbol(infix, ERROR, 'E');
    *good = 0;
  }
  return src_str;
}

void make_unary_operator(struct ll_node *s) {
  s->state = UNARYOPERATOR;
  s->datum = (s->datum == '+') ? 'p' : 'm';
}

int check_syntax(struct ll_node *last, struct ll_node *before) {
  int good = 1;
  if (last->state == ERROR) good = 0;
  if (before->state == UNARYOPERATOR &&
      (last->state == OPERATOR || last->state == R_BRACKET))
    good = 0;

  if (last->state == before->state && last->state != L_BRACKET &&
      last->state != R_BRACKET)
    good = 0;

  if (before->state == OPERATOR && last->state == R_BRACKET) good = 0;
  if (before->state == R_BRACKET &&
      (last->state != OPERATOR && last->state != R_BRACKET))
    good = 0;

  if (before->state == OPERAND && last->state == VARIABLE) good = 0;
  if (before->state == VARIABLE && last->state == OPERAND) good = 0;

  if (before->state != OPERATOR && before->state != UNARYOPERATOR &&
      before->state != L_BRACKET && last->state == FUNCTION)
    good = 0;

  return good;
}

struct expr *expr_from_string(char *a, int *good) {
  char *p = a;
  struct expr *infix = NULL;
  int parents = 0;
  *good = 1;
  if (p && *p) {
    p = one_expr_from_string(p, &infix, good, &parents);  // First symbol
    struct ll_node *last = infix->head;
    if (last->state == OPERATOR && (last->datum == '-' || last->datum == '+'))
      make_unary_operator(last);
    if (infix->head->state == OPERATOR) {
      *good = 0;
    } else {
      while (*p && *good) {  // Rest of the string
        p = one_expr_from_string(p, &infix, good, &parents);
        struct ll_node *before;
        last = ll_last_node(infix->head);
        before = ll_before_last_node(infix->head);
        if ((last->state == OPERATOR &&
             (last->datum == '-' || last->datum == '+')) &&
            ((before->state == OPERATOR) || (before->state == L_BRACKET)))
          make_unary_operator(last);
        *good = check_syntax(last, before);
      }
    }
  } else {
    infix = expr_new();
    expr_add_symbol(infix, ERROR, 'E');
    *good = 0;
  }
  if (parents != 0) *good = 0;
  return infix;
}

char *one_expr_from_string(char *str, struct expr **infix_to_fill, int *good,
                           int *parents) {
  struct expr *infix;
  char *src_str = str;

  if (*infix_to_fill == NULL) {
    infix = expr_new();
  } else {
    infix = *infix_to_fill;
  }

  if (src_str && *src_str) {
    while (*src_str && is_space(*src_str)) ++src_str;  // Skip spaces
    if (is_oper(*src_str)) {                           // Operator
      if (*src_str == '(') {                           // Left bracket
        (*parents)++;
        expr_add_symbol(infix, L_BRACKET, *src_str);
      } else if (*src_str == ')') {  // right bracket
        (*parents)--;
        if ((*parents) >= 0) {
          expr_add_symbol(infix, R_BRACKET, *src_str);
        } else {  // Right bracket with no left
          expr_add_symbol(infix, ERROR, 'E');
          *good = 0;
        }
      } else {  // Any other operator
        expr_add_symbol(infix, OPERATOR, *src_str);
      }
      src_str++;
    } else if (is_alpha(*src_str)) {  // Function or variable
      src_str = expr_add_function(infix, src_str, good);
    } else if (is_digit(*src_str)) {  // Digit
      expr_add_symbol(infix, OPERAND, get_operand(src_str));
      while (*src_str && is_digit(*src_str)) {
        src_str++;
      }
    } else {                     // EOL
      if (infix->length == 0) {  // If meet EOL and expr is empty
        expr_add_symbol(infix, ERROR, 'E');
        *good = 0;
      }
      if (*src_str != 0) src_str++;
    }
  }
  *infix_to_fill = infix;
  return src_str;
}
