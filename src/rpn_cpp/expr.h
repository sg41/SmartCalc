#ifndef SRC_RPR_EXPR_H_
#define SRC_RPR_EXPR_H_

#include <math.h>

#include <list>
#include <map>
#include <regex>
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

using func_type = double (*)(double, double);

class TokenData {
 public:
  token_type t;
  precedence p;
  func_type call;
};

class ExprToken {
 public:
  ExprToken(){};
  virtual ~ExprToken(){};
  explicit ExprToken(token_type s, double d) : state_(s), data_(d){};
  virtual double &data() { return data_; };
  // virtual const double &cdata() const { return data_; };
  token_type state() const { return state_; };
  void setState(token_type s) { state_ = s; };
  virtual double func(double, double) { return data_; };
  virtual precedence priorty() { return ADD_SCORE; };
  virtual string name() { return to_string(data_); };

 protected:
  token_type state_ = ERROR;
  double data_ = 0;
};

class VarExprToken : public ExprToken {
 public:
  using ExprToken::ExprToken;
  VarExprToken(token_type s, string n) : ExprToken(s, 0.0), var_name_(n){};
  // double &data() override { return *var_ref_; };
  // const double &cdata() const override { return *var_ref_; };
  string name() override { return var_name_; };

 protected:
  double *var_ref_ = nullptr;
  string var_name_ = "x";
};

class FuncExprToken : public ExprToken {
 public:
  using ExprToken::ExprToken;
  FuncExprToken(string n, token_type s, precedence p, func_type f)
      : ExprToken(s, 0.0), priority_(p), fnc_(f), name_(n){};
  FuncExprToken(char c, token_type s, precedence p, func_type f)
      : ExprToken(s, 0.0), priority_(p), fnc_(f), name_(string("") + c){};
  FuncExprToken(string n, TokenData d)
      : ExprToken(d.t, 0.0), priority_(d.p), fnc_(d.call), name_(n){};

  double func(double a, double b) override {
    return (fnc_ != nullptr) ? fnc_(a, b)
                             : throw std::invalid_argument("Wrong token");
  };
  precedence priorty() override { return priority_; };
  string name() override { return name_; };

 protected:
  precedence priority_ = ADD_SCORE;
  func_type fnc_ = nullptr;
  string name_;
};

using namespace std;
class ExprSyntax {
 public:
  ExprSyntax() { count_length(); };
  int is_space(char o) { return spaces_.find(o) != std::string::npos ? 1 : 0; };
  int is_space(string o) {
    return spaces_.find(o[0]) != std::string::npos ? o.size() : 0;
  };
  int is_L_bracket(string o) {
    return L_brackets_.find(o) != std::string::npos ? o.size() : 0;
  };
  int is_R_bracket(string o) {
    return R_brackets_.find(o) != string::npos ? o.size() : 0;
  };
  int is_unary_operator(string o) {
    return unary_operators_.count(o) ? o.size() : 0;
  };
  int is_operand(string o) {
    int res = 0;
    const std::regex base_regex("[0-9]*[.,]?[0-9]+(?:[eE][-+]?[0-9]+)?");
    std::smatch base_match;
    if (std::regex_match(o, base_match, base_regex)) {
      // std::ssub_match base_sub_match = base_match[0];
      res = base_match[0].str().size();
    }
    return res;
  };
  double get_operand(string s) { return std::atof(s.data()); };

  int is_variable(string o) { return variable_.count(o) ? o.size() : 0; };
  int is_operator(string o) { return operators_.count(o) ? o.size() : 0; };
  int is_function(string o) { return functions_.count(o) ? o.size() : 0; };
  TokenData get_data(string s, token_type t = ERROR) {
    TokenData d{};
    if (t == OPERATOR) d = operators_[s];
    if (t == UNARYOPERATOR) d = unary_operators_[s];
    if (t == FUNCTION) d = functions_[s];
    return d;
  };

 protected:
  void count_length() {
    for (auto l : functions_) {
      f_length_.insert(l.first.size());
    }
  };
  set<int> f_length_, uop_length_, op_length_;
  string spaces_ = " \t\n\r";
  string L_brackets_ = {"("};
  string R_brackets_ = {")"};
  set<string> variable_ = {"x", "X"};
  map<string, TokenData> unary_operators_{
      {"+", {UNARYOPERATOR, UOP_SCORE, [](double a, double) { return a; }}},
      {"-", {UNARYOPERATOR, UOP_SCORE, [](double a, double) { return -a; }}}};
  map<string, TokenData> operators_{
      {"+", {OPERATOR, ADD_SCORE, [](double a, double b) { return a + b; }}},
      {"-", {OPERATOR, SUB_SCORE, [](double a, double b) { return a - b; }}},
      {"/",
       {OPERATOR, DIV_SCORE,
        [](double a, double b) { return (b != 0) ? a / b : NAN; }}},
      {"*", {OPERATOR, MUL_SCORE, [](double a, double b) { return a * b; }}},
      {"%",
       {OPERATOR, DIV_SCORE, [](double a, double b) { return fmod(a, b); }}},
      {"^",
       {OPERATOR, EXP_SCORE, [](double a, double b) { return pow(a, b); }}}};
  map<string, TokenData> functions_{
      {"sin(", {OPERATOR, FUN_SCORE, [](double a, double) { return sin(a); }}},
      {"cos(", {OPERATOR, FUN_SCORE, [](double a, double) { return cos(a); }}},
      {"tan(", {OPERATOR, FUN_SCORE, [](double a, double) { return tan(a); }}},
      {"abs(", {OPERATOR, FUN_SCORE, [](double a, double) { return abs(a); }}},
      {"log(",
       {OPERATOR, FUN_SCORE, [](double a, double) { return log10(a); }}},
      {"ln(", {OPERATOR, FUN_SCORE, [](double a, double) { return log(a); }}},
      {"sqrt(",
       {OPERATOR, FUN_SCORE, [](double a, double) { return sqrt(a); }}},
      {"asin(",
       {OPERATOR, FUN_SCORE, [](double a, double) { return asin(a); }}},
      {"acos(",
       {OPERATOR, FUN_SCORE, [](double a, double) { return acos(a); }}},
      {"atan(",
       {OPERATOR, FUN_SCORE, [](double a, double) { return atan(a); }}}};
};

using namespace std;
class TokenList : public list<ExprToken *> {
 public:
  using list<ExprToken *>::list;
  TokenList(ExprSyntax *s) {
    syntax = s;
    brackets = 0;
  };
  void make_list(string str);

 protected:
  ExprToken *create_token(string str, token_type t);
  std::pair<int, token_type> find_token(string str);
  int skip_spaces(string str);
  void make_unary_operator();
  bool check_syntax();
  ExprToken *before_back() {
    (back() == front()) ? nullptr : *(prev(end(), 2));
  };
  void make_unary_operator();
  int brackets = 0;
  ExprSyntax *syntax;
};

#endif  // SRC_RPR_EXPR_H_
