#ifndef SRC_RPR_EXPR_H_
#define SRC_RPR_EXPR_H_

#include <math.h>

#include <list>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <type_traits>

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

enum precedence {
  ADD_SCORE = 2,
  SUB_SCORE = 2,
  MUL_SCORE = 4,
  DIV_SCORE = 4,
  FUN_SCORE = 10,
  EXP_SCORE = 6,
  UOP_SCORE = 8,
  L_SCORE = 12,
  R_SCORE = 12,
};

template <class T>
class TokenData {
 public:
  token_type t;
  precedence p;
  T call;
};

class ExprToken {
 public:
  ExprToken(){};
  virtual ~ExprToken(){};
  explicit ExprToken(token_type s, double d) : state_(s), data_(d){};
  virtual double &data() { return data_; };
  virtual const double &cdata() const { return data_; };
  token_type state() const { return state_; };
  void setState(token_type s) { state_ = s; };
  virtual double oper(double, double) { return data_; };
  virtual double func(double) { return data_; };
  virtual precedence priorty() { return ADD_SCORE; };

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
using oper_type = double (*)(double, double);
class FuncExprToken : public ExprToken {
 public:
  using ExprToken::ExprToken;
  FuncExprToken(token_type s, precedence p, func_type v)
      : ExprToken(s, 0.0), priority_(p), fnc_(v){};
  FuncExprToken(token_type s, precedence p, oper_type v)
      : ExprToken(s, 0.0), priority_(p), opr_(v){};
  FuncExprToken(TokenData<func_type> d)
      : ExprToken(d.t, 0.0), priority_(d.p), fnc_(d.call){};

  double func(double a) override {
    return (fnc_ != nullptr) ? fnc_(a)
                             : throw std::invalid_argument("Wrong token");
  };
  double oper(double a, double b) override {
    return (opr_ != nullptr) ? opr_(a, b)
                             : throw std::invalid_argument("Wrong token");
  };
  precedence priorty() override { return priority_; };

 protected:
  precedence priority_ = ADD_SCORE;
  oper_type opr_ = nullptr;
  func_type fnc_ = nullptr;
};

using namespace std;
class ExprSyntax {
 public:
  friend class TokenList;
  ExprSyntax() { count_length(); };
  // bool is_unary_operator(char o) { return unary_operators_.count(o); };
  // bool is_unary_operator(string o) { return unary_operators_.count(o[0]); };
  // bool is_operator(char o) { return operators_.count(o); };
  // bool is_operator(string o) { return operators_.count(o[0]); };
  // bool is_function(string o) { return functions_.count(o); };
  int is_space(char o) { return spaces_.find(o) != std::string::npos ? 1 : 0; };
  int is_space(string o) {
    return spaces_.find(o[0]) != std::string::npos ? 1 : 0;
  };
  int is_bracket(char o) {
    return brackets_.find(o) != std::string::npos ? 1 : 0;
  };
  int is_bracket(string o) {
    return brackets_.find(o[0]) != string::npos ? 1 : 0;
  };
  int is_unary_operator(string o) {
    return tokens_[UNARYOPERATOR].count(o.substr(0, 1)) ? 1 : 0;
  };
  int is_operator(string o) {
    return tokens_[OPERATOR].count(o.substr(0, 1)) ? 1 : 0;
  };
  int is_function(string o) {
    int res = 0;
    for (auto l = length_.rend(), l != length_.rbegin(), ++l)
      if (tokens_[FUNCTION].count(o.substr(0, *l))) {
        res = l;
        break;
      }
    return res;
  };
  TokenData get_data(string s, token_type t = ERROR) {
    TokenData d{};
    if (t != ERROR) d = tokens_[t][s];
    return d;
  };

 protected:
  void count_length() {
    for (auto l : functions_) {
      length_.insert(l.first.size());
    }
  };
  set<int> length_;
  string spaces_ = " \t\n\r";
  pair<char, char> brackets_ = {'(', ')'};  //!
  map<token_type, map<string, TokenData<func_type>>> tokens_ = {
      // map<char, TokenData<func_type>> unary_operators_{
      {UNARYOPERATOR,
       {"+", {OPERATOR, UOP_SCORE, [](double a) { return a; }}},
       {"-", {OPERATOR, UOP_SCORE, [](double a) { return -a; }}}},
      // map<char, TokenData<oper_type>> operators_{
      {OPERATOR,
       {"+", {OPERATOR, ADD_SCORE, [](double a, double b) { return a + b; }}},
       {"-", {OPERATOR, SUB_SCORE, [](double a, double b) { return a - b; }}},
       {"/",
        {OPERATOR, DIV_SCORE,
         [](double a, double b) { return (b != 0) ? a / b : NAN; }}},
       {"*", {OPERATOR, MUL_SCORE, [](double a, double b) { return a * b; }}},
       {"%",
        {OPERATOR, DIV_SCORE, [](double a, double b) { return fmod(a, b); }}},
       {"^",
        {OPERATOR, EXP_SCORE, [](double a, double b) { return pow(a, b); }}}},
      // map<string, TokenData<func_type>> functions_{
      {FUNCTION,
       {"sin(", {OPERATOR, FUN_SCORE, sin}},
       {"cos(", {OPERATOR, FUN_SCORE, cos}},
       {"tan(", {OPERATOR, FUN_SCORE, tan}},
       {"abs(", {OPERATOR, FUN_SCORE, abs}},
       {"log(", {OPERATOR, FUN_SCORE, log10}},
       {"ln(", {OPERATOR, FUN_SCORE, log}},
       {"sqrt(", {OPERATOR, FUN_SCORE, sqrt}},
       {"asin(", {OPERATOR, FUN_SCORE, asin}},
       {"acos(", {OPERATOR, FUN_SCORE, acos}},
       {"atan(", {OPERATOR, FUN_SCORE, atan}}}};
};

using namespace std;
class TokenList : public list<ExprToken *> {
 public:
  using list<ExprToken *>::list;
  TokenList(ExprSyntax *s) { syntax = s; };
  void make_list(string str);

 protected:
  ExprToken *create_token(string str);
  int find_token(string str);
  int skip_spaces(string str);
  void make_unary_operator();
  bool check_syntax();
  // string str;
  ExprSyntax *syntax;
};

#endif  // SRC_RPR_EXPR_H_
