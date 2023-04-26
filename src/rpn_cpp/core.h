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
  ~CalcCore() { delete syntax; };
  //! void set_syntax(ExprSyntax *s) { syntax = s; };
  void make_rpn_expr(const std::string &s) {
    if (!rpn_expr_.empty()) rpn_expr_.clear_and_delete();
    TokenList infix(syntax);
    infix.make_infix_list(s);
    move_infix_to_rpn(infix);
  };
  double calc(double x) { return (rpn_expr_.size() > 0) ? rpn_reduce(x) : 0; };
  int get_expr_size() { return rpn_expr_.size(); };

 protected:
  double rpn_reduce(double x);
  void move_infix_to_rpn(TokenList &infix);
  void stack_to_rpn(std::stack<ExprToken *> &opstack) {
    rpn_expr_.push_back(opstack.top());
    opstack.pop();
  };

 protected:
  TokenList rpn_expr_;
  ExprSyntax *syntax = nullptr;
};

#endif  // SRC_RPR_CORE_H_
