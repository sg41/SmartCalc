#ifndef _SRC_RPN_CPP_CORE_H_
#define _SRC_RPN_CPP_CORE_H_
#include <math.h>
#include <stdarg.h>
#include <string.h>

#include <list>
#include <string>

#include "expr.h"

namespace s21 {
/// @brief used to perform expressions calculations
/// based on specified syntax
class CalcCore {
 public:
  /// @brief constructor creates expression syntax
  CalcCore() : syntax_(new ExprSyntax()){};
  ~CalcCore() { delete syntax_; };
  /// @brief  copy constructor
  /// @param c other CalcCore object to copy syntax from
  CalcCore(const CalcCore &c) : syntax_(new ExprSyntax(*c.syntax_)){};
  /// @brief Move constructor
  /// @param c other CalcCore object to move syntax from
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
  /// @brief creates Reverse Polish Notation expression list
  /// to be used for quick expression evaluation
  /// @param s source std::string, containing expression in standart infix
  /// notation
  void makeRpnExpr(const std::string &s) {
    if (!rpn_expr_.empty()) rpn_expr_.clearAndDelete();
    TokenList infix(syntax_);
    infix.makeInfixList(s);
    moveInfixToRpn(infix);
  };
  /// @brief perform expression calculation using previously created RPN
  /// expression list
  /// @param x variable used in expression
  /// @return result of expression evlaluation
  double calc(double x) {
    return (rpn_expr_.size() > 0) ? rpnCalculate(x) : 0;
  };

  int getExprSize() { return rpn_expr_.size(); };

 protected:
  double rpnCalculate(double x);
  void moveInfixToRpn(TokenList &infix);
  void moveStackToRpn(std::stack<ExprToken *> &opstack) {
    rpn_expr_.push_back(opstack.top());
    opstack.pop();
  };

 protected:
  TokenList rpn_expr_;            //< RPN expression list
  ExprSyntax *syntax_ = nullptr;  //< Expression syntax
};
}  // namespace s21
#endif  // _SRC_RPN_CPP_CORE_H_
