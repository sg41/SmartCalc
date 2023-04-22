#include "core.h"

#include <math.h>

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
        k.emplace(OPERAND, i->oper(a, b));
      else if (i->state() == UNARYOPERATOR || i->state() == FUNCTION)
        k.emplace(OPERAND, i->func(a));
      else
        throw std::invalid_argument("Wrong expressin token");
    }
  }
  double res = k.top().data();
  return res;
}

void CalcEngine::expr_from_string(std::list<ExprToken *> &infix,
                                  std::string s) {
  const char *p = s.c_str();
  int parents = 0;
  int good = 1;
  if (p && *p) {
    p = one_token_from_string(infix, p, &good, &parents);  // First symbol
    ExprToken *last = infix.back();
    if (last->state() == OPERATOR &&
        (last->data() == '-' || last->data() == '+'))
      make_unary_operator(infix);
    last = infix.back();
    if (last->state() == OPERATOR) {
      good = 0;
    } else {
      while (*p && good) {  // Rest of the string
        p = one_token_from_string(infix, p, &good, &parents);
        ExprToken *before;
        last = infix.back();
        before = *(prev(infix.end(), 2));
        if ((last->state() == OPERATOR &&
             (last->data() == '-' || last->data() == '+')) &&
            ((before->state() == OPERATOR) || (before->state() == L_BRACKET)))
          make_unary_operator(infix);
        last = infix.back();
        good = check_syntax(last, before);
      }
    }
  } else {
    throw std::invalid_argument("Wrong expresion string");
    // infix = expr_new();
    // infix.push_back(new FuncExprToken( ERROR, 'E');
    // good = 0;
  }
  if (parents != 0) throw std::invalid_argument("Wrong () pairs");
};

// void CalcEngine::expr_shunt(std::list<ExprToken *> &infix){};

const char *CalcEngine::one_token_from_string(std::list<ExprToken *> &infix,
                                              const char *str, int *good,
                                              int *parents) {
  if (str && *str) {
    while (*str && is_space(*str)) ++str;  // Skip spaces
    if (is_oper(*str)) {                   // Operator
      if (*str == '(') {                   // Left bracket
        (*parents)++;
        infix.push_back(new ExprToken(L_BRACKET, *str));
      } else if (*str == ')') {  // right bracket
        (*parents)--;
        if ((*parents) >= 0) {
          infix.push_back(new ExprToken(R_BRACKET, *str));
        } else {  //! Right bracket with no left
          infix.push_back(new ExprToken(ERROR, 'E'));
          *good = 0;
        }
      } else {  // Any other operator
        infix.push_back(new ExprToken(OPERATOR, *str));
      }
      str++;
    } else if (is_alpha(*str)) {  // Function or variable
      str = expr_add_function(infix, str, good);
    } else if (is_digit(*str)) {  // Digit
      infix.push_back(new ExprToken(OPERAND, get_operand(str)));
      while (*str && is_digit(*str)) {
        str++;
      }
    } else {                    // EOL
      if (infix.size() == 0) {  //! If meet EOL and expr is empty
        infix.push_back(new ExprToken(ERROR, 'E'));
        *good = 0;
      }
      if (*str != 0) str++;
    }
  }
  // *infix_to_fill = infix;
  return str;
}

const char *CalcEngine::expr_add_function(std::list<ExprToken *> &infix,
                                          const char *src_str, int *good) {
  if (strncmp(src_str, "sin(", 4) == 0) {
    infix.push_back(new FuncExprToken(FUNCTION, sin));
    src_str += 3;
  } else if (strncmp(src_str, "cos(", 4) == 0) {
    infix.push_back(new FuncExprToken(FUNCTION, cos));
    src_str += 3;
  } else if (strncmp(src_str, "tan(", 4) == 0) {
    infix.push_back(new FuncExprToken(FUNCTION, tan));
    src_str += 3;
  } else if (strncmp(src_str, "abs(", 4) == 0) {
    infix.push_back(new FuncExprToken(FUNCTION, abs));
    src_str += 3;
  } else if (strncmp(src_str, "log(", 4) == 0) {
    infix.push_back(new FuncExprToken(FUNCTION, log10));
    src_str += 3;
  } else if (strncmp(src_str, "ln(", 3) == 0) {
    infix.push_back(new FuncExprToken(FUNCTION, log));
    src_str += 2;
  } else if (strncmp(src_str, "mod", 3) == 0) {
    infix.push_back(new OperExprToken(OPERATOR, fmod));
    src_str += 3;
  } else if (strncmp(src_str, "sqrt(", 5) == 0) {
    infix.push_back(new FuncExprToken(FUNCTION, sqrt));
    src_str += 4;
  } else if (strncmp(src_str, "asin(", 5) == 0) {
    infix.push_back(new FuncExprToken(FUNCTION, asin));
    src_str += 4;
  } else if (strncmp(src_str, "acos(", 5) == 0) {
    infix.push_back(new FuncExprToken(FUNCTION, acos));
    src_str += 4;
  } else if (strncmp(src_str, "atan(", 5) == 0) {
    infix.push_back(new FuncExprToken(FUNCTION, atan));
    src_str += 4;
  } else if (*src_str == 'x' || *src_str == 'X') {
    infix.push_back(new VarExprToken(VARIABLE, 'x'));
    src_str++;
  } else {
    while (is_alpha(*src_str)) {
      src_str++;
    }
    infix.push_back(new ExprToken(ERROR, 'E'));
    *good = 0;
  }
  return src_str;
}

int CalcEngine::check_syntax(ExprToken *last, ExprToken *before) {
  int good = 1;
  if (last->state() == ERROR) good = 0;
  if (before->state() == UNARYOPERATOR &&
      (last->state() == OPERATOR || last->state() == R_BRACKET))
    good = 0;

  if (last->state() == before->state() && last->state() != L_BRACKET &&
      last->state() != R_BRACKET)
    good = 0;

  if (before->state() == OPERATOR && last->state() == R_BRACKET) good = 0;
  if (before->state() == R_BRACKET &&
      (last->state() != OPERATOR && last->state() != R_BRACKET))
    good = 0;

  if (before->state() == OPERAND && last->state() == VARIABLE) good = 0;
  if (before->state() == VARIABLE && last->state() == OPERAND) good = 0;

  if (before->state() != OPERATOR && before->state() != UNARYOPERATOR &&
      before->state() != L_BRACKET && last->state() == FUNCTION)
    good = 0;

  return good;
}
