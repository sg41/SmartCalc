#ifndef SRC_RPR_CORE_H_
#define SRC_RPR_CORE_H_
#include <math.h>
#include <stdarg.h>

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
  FUNCTION,
  VARIABLE,
  L_BRACKET,
  R_BRACKET,
  ERROR
};

class ExprToken {
 public:
  ExprToken(){};
  virtual ~ExprToken(){};
  ExprToken(token_type s, double d) : state_(s), data_(d){};
  virtual double &data() { return data_; };
  virtual const double &cdata() const { return data_; };
  token_type state() const { return state_; };
  void setState(token_type s) { state_ = s; };
  virtual double oper(double, double) { return data_; };
  virtual double func(double) { return data_; };

 protected:
  token_type state_ = ERROR;
  double data_ = 0;
};

class VarExprToken : public ExprToken {
 public:
  using ExprToken::ExprToken;
  VarExprToken(token_type s, double *v, char n)
      : ExprToken(s, 0), var_ref_(v), var_name_(n){};
  double &data() override { return *var_ref_; };
  const double &cdata() const override { return *var_ref_; };

 protected:
  double *var_ref_ = nullptr;
  char var_name_ = 'x';
};

using func_type = double (*)(double);
class FuncExprToken : public ExprToken {
 public:
  using ExprToken::ExprToken;
  FuncExprToken(token_type s, func_type v, int n = 1)
      : ExprToken(s, 0), fnc_(v), n_args_(n){};
  double func(double a) override { return fnc_(a); };

 protected:
  func_type fnc_ = nullptr;
  int n_args_ = 1;
};

using oper_type = double (*)(double, double);
class OperExprToken : public ExprToken {
 public:
  using ExprToken::ExprToken;
  OperExprToken(token_type s, oper_type v, int n = 2)
      : ExprToken(s, 0), fnc_(v), n_args_(n){};
  double oper(double a, double b) override { return fnc_(a, b); };

 protected:
  oper_type fnc_;
  int n_args_ = 2;
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
  CalcEngine(){};
  ~CalcEngine() {
    for (auto l : rpn_expr_) delete l;
  };
  void make_rpn_expr(std::string s) {
    std::list<ExprToken *> infix;
    expr_from_string(infix, s);
    expr_shunt(infix);
  };
  double calc(double x) {
    double res = 0;
    if (rpn_expr_.size() > 0) res = rpn_reduce(x);
    return res;
  };

 public:
  std::list<ExprToken *> rpn_expr_;
};

#endif  // SRC_RPR_CORE_H_
