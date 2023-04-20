#ifndef SRC_RPR_CORE_H_
#define SRC_RPR_CORE_H_
#include <math.h>

#include <list>
#include <string>

// #include "expr.h"
#ifndef MAXSTR
#define MAXSTR 10000
#endif

enum token_type {
  OPERAND,
  OPERATOR,
  UNARYOPERATOR,
  VARIABLE,
  L_BRACKET,
  R_BRACKET,
  FUNCTION,
  ERROR
};

class ExprToken {
 public:
  ExprToken(){};
  ExprToken(token_type s, double d) : state_(s), data_(d){};
  virtual double &data() { return data_; };
  const token_type state() const { return state_; };
  void setState(token_type s) { state_ = s; };

 protected:
  token_type state_ = ERROR;
  double data_ = 0;
};

class VarExprToken : public ExprToken {
  using ExprToken::ExprToken;
  VarExprToken(token_type s, double &v) : ExprToken(s, 0), var_ref_(v){};
  double &data() override { return var_ref_; };

 protected:
  double &var_ref_;
};

class CalcEngine {
 public:
  // Functions
  static const int sin_FUNCTION = 's';
  static const int cos_FUNCTION = 'c';
  static const int tan_FUNCTION = 't';
  static const int ctg_FUNCTION = 'g';
  static const int abs_FUNCTION = 'a';
  static const int log_FUNCTION = 'l';
  static const int ln_FUNCTION = 'e';
  static const int sqrt_FUNCTION = 'q';
  static const int asin_FUNCTION = 'i';
  static const int acos_FUNCTION = 'o';
  static const int atan_FUNCTION = 'n';
  static const int error_FUNCTION = 'E';
  // Operators:
  static const int mod_FUNCTION = '%';
  static const int UNARY_PLUS = 'p';
  static const int UNARY_MINUS = 'm';

  static const int ADD_SCORE = 2;
  static const int SUB_SCORE = 2;
  static const int MULT_SCORE = 4;
  static const int DIV_SCORE = 4;
  static const int FUNC_SCORE = 10;
  static const int EXP_SCORE = 6;
  static const int U_SCORE = 8;
  static const int L_SCORE = 12;
  static const int R_SCORE = 12;

 protected:
  double rpn_reduce(double x);
  void expr_from_string(std::list<ExprToken *> &infix, std::string s);
  void expr_shunt(std::list<ExprToken *> &infix);

 public:
  CalcEngine() { rpn_expr_.clear(); };
  int make_rpn_expr(std::string s) {
    std::list<ExprToken *> infix;
    expr_from_string(infix, s);
    expr_shunt(infix);
  };
  double calc(double x) {
    double res = 0;
    if (rpn_expr_.size() > 0) res = rpn_reduce(x);
    return res;
  };

 protected:
  std::list<ExprToken *> rpn_expr_;
};

#endif  // SRC_RPR_CORE_H_
