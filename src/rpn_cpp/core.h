#ifndef SRC_RPR_CORE_H_
#define SRC_RPR_CORE_H_
#include <math.h>
#include <stdarg.h>
#include <string.h>

#include <list>
#include <string>

#include "expr.h"

class CalcCore {
 public:
  void expr_from_string(std::list<ExprToken *> &infix, std::string s);

 protected:
  double rpn_reduce(double x);
  void expr_shunt(std::list<ExprToken *> &infix);
  bool is_oper(char c) { return (c != 0 && strchr("+-/*%^()", c)); };
  bool is_space(char c) { return (strchr(" \t\n\r", c)); };  // including 0
  bool is_digit(char c) { return (c >= '0' && c <= '9') || c == '.'; };
  bool is_alpha(char c) { return c >= 'A' && c <= 'z'; };
  double get_operand(const char *a) { return std::atof(a); };
  int check_syntax(ExprToken *last, ExprToken *before);
  void make_unary_operator(std::list<ExprToken *> &infix) {
    ExprToken *t = infix.back();
    infix.pop_back();
    infix.push_back(new FuncExprToken(
        t->data(), UNARYOPERATOR, UOP_SCORE,
        (t->data() == '+') ? [](double a, double) { return a; }
                           : [](double a, double) { return -a; }));
    delete t;
  }
  const char *one_token_from_string(std::list<ExprToken *> &infix,
                                    const char *str, int *good, int *parents);
  const char *expr_add_function(std::list<ExprToken *> &infix,
                                const char *src_str, int *good);

 public:
  CalcCore(){};
  ~CalcCore() {
    for (auto t : rpn_expr_) delete t;
  };
  void set_syntax(ExprSyntax *s) { syntax = s; };
  void make_rpn_expr(std::string s) {
    std::list<ExprToken *> infix;
    expr_from_string(infix, s);
    expr_shunt(infix);
    //! for (auto t : infix) delete t;
  };
  double calc(double x) {
    double res = 0;
    if (rpn_expr_.size() > 0) res = rpn_reduce(x);
    return res;
  };

 public:
  std::list<ExprToken *> rpn_expr_;
  ExprSyntax *syntax = nullptr;
};

#endif  // SRC_RPR_CORE_H_
