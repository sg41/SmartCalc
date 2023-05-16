#ifndef _SRC_RPN_CPP_CORE_H_
#define _SRC_RPN_CPP_CORE_H_
#include <math.h>
#include <stdarg.h>
#include <string.h>

#include <list>
#include <string>

#include "expr.h"
namespace s21 {
class CalcCore {
 public:
  CalcCore() : syntax_(new ExprSyntax()){};
  ~CalcCore() { delete syntax_; };
  CalcCore(const CalcCore &c) : syntax_(new ExprSyntax(*c.syntax_)){};
  CalcCore(CalcCore &&c) {
    syntax_ = c.syntax_;
    c.syntax_ = nullptr;
  };
  CalcCore &operator=(const CalcCore &c) {
    if (this != &c) *syntax_ = *c.syntax_;
    return *this;
  }
  CalcCore &operator=(CalcCore &&c) {
    if (this != &c) {
      delete syntax_;
      syntax_ = c.syntax_;
      c.syntax_ = nullptr;
    }
    return *this;
  }
  void makeRpnExpr(const std::string &s) {
    if (!rpn_expr_.empty()) rpn_expr_.clearAndDelete();
    TokenList infix(syntax_);
    infix.makeInfixList(s);
    moveInfixToRpn(infix);
  };
  double calc(double x) {
    return (rpn_expr_.size() > 0) ? rpnCalculate(x) : 0;
  };
  int get_expr_size() { return rpn_expr_.size(); };

 protected:
  double rpnCalculate(double x);
  void moveInfixToRpn(TokenList &infix);
  void moveStackToRpn(std::stack<ExprToken *> &opstack) {
    rpn_expr_.push_back(opstack.top());
    opstack.pop();
  };

 protected:
  TokenList rpn_expr_;
  ExprSyntax *syntax_ = nullptr;
};
}  // namespace s21
#endif  // _SRC_RPN_CPP_CORE_H_
