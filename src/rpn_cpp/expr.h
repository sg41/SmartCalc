#ifndef SRC_RPR_EXPR_H_
#define SRC_RPR_EXPR_H_

#include <math.h>

#include <map>

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
  // explicit ExprToken(token_type s, char d) : state_(s), data_((double)d){};
  explicit ExprToken(token_type s, double d) : state_(s), data_(d){};
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
      : ExprToken(s, 0.0), var_ref_(v), var_name_(n){};
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
      : ExprToken(s, 0.0), fnc_(v), n_args_(n){};
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
      : ExprToken(s, 0.0), fnc_(v), n_args_(n){};
  double oper(double a, double b) override { return fnc_(a, b); };

 protected:
  oper_type fnc_;
  int n_args_ = 2;
};

using namespace std;
class ExprSyntax {
 public:
  ExprToken *createToken(string::iterator s);

 protected:
  string spaces_ = " \t\n\r";
  string brackets_ = "()";
  map<string, oper_type> operators_{
      {"+", [](double a, double b) { return a + b; }},
      {"-", [](double a, double b) { return a - b; }},
      {"/", [](double a, double b) { return (b != 0) ? a / b : NAN; }},
      {"*", [](double a, double b) { return a * b; }},
      {"%", [](double a, double b) { return fmod(a, b); }},
      {"^", [](double a, double b) { return pow(a, b); }},
      {"mod", fmod}};
  map<string, func_type> functions_{
      {"sin(", sin},   {"cos(", cos},  {"tan(", tan},   {"abs(", abs},
      {"log(", log10}, {"ln(", log},   {"sqrt(", sqrt}, {"asin(", asin},
      {"acos(", acos}, {"atan(", atan}};
};

#endif  // SRC_RPR_EXPR_H_
