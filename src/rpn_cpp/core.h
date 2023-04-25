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
  CalcCore() : syntax(new ExprSyntax()){};
  ~CalcCore() {
    for (auto t : rpn_expr_) delete t;
    delete syntax;
  };
  //! void set_syntax(ExprSyntax *s) { syntax = s; };
  void make_rpn_expr(const std::string &s) {
    TokenList infix(syntax);
    infix.make_list(s);
    expr_shunt(infix);
  };
  double calc(double x) {
    double res = 0;
    if (rpn_expr_.size() > 0) res = rpn_reduce(x);
    return res;
  };

 protected:
  double rpn_reduce(double x);
  void expr_shunt(TokenList &infix);
  void stack_to_rpn(std::stack<ExprToken *> &opstack);

 public:
  std::list<ExprToken *> rpn_expr_;
  ExprSyntax *syntax = nullptr;
};

#endif  // SRC_RPR_CORE_H_
