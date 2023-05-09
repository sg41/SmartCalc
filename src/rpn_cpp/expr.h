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

#ifdef NDEBUG
#define DBPRINT(fmt, ...) \
  do {                    \
  } while (0)
#else /* Not NDEBUG.  */
#define DBPRINT(fmt, ...)              \
  do {                                 \
    fprintf(stderr, fmt, __VA_ARGS__); \
  } while (0)
#endif

namespace s21 {

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
  NO_SCORE = 0,
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
  token_type state() const { return state_; };
  void setState(token_type s) { state_ = s; };
  virtual double func(double, double) { return data_; };
  virtual precedence priority() { return ADD_SCORE; };
  virtual std::string name() { return std::to_string(data_); };

 protected:
  token_type state_ = ERROR;
  double data_ = 0;
};

class VarExprToken : public ExprToken {
 public:
  using ExprToken::ExprToken;
  VarExprToken(token_type s, const std::string &n)
      : ExprToken(s, 0.0), var_name_(n){};
  std::string name() override { return var_name_; };

 protected:
  double *var_ref_ = nullptr;
  std::string var_name_ = "x";
};

class FuncExprToken : public ExprToken {
 public:
  using ExprToken::ExprToken;
  FuncExprToken(const std::string &n, token_type s, precedence p, func_type f)
      : ExprToken(s, 0.0), priority_(p), fnc_(f), name_(n){};
  FuncExprToken(char c_name, token_type s, precedence p, func_type f)
      : ExprToken(s, 0.0),
        priority_(p),
        fnc_(f),
        name_(std::string("") + c_name){};
  FuncExprToken(const std::string &n, TokenData d)
      : ExprToken(d.t, 0.0), priority_(d.p), fnc_(d.call), name_(n){};

  double func(double a, double b) override {
    return (fnc_ != nullptr) ? fnc_(a, b)
                             : throw std::invalid_argument("Wrong token");
  };
  precedence priority() override { return priority_; };
  std::string name() override { return name_; };

 protected:
  precedence priority_ = ADD_SCORE;
  func_type fnc_ = nullptr;
  std::string name_;
};

class ExprSyntax {
 public:
  ExprSyntax() { count_length(); };
  int is_operand(const std::string &o);
  int is_space(char o) { return spaces_.find(o) != std::string::npos ? 1 : 0; };
  int is_space(const std::string &o) {
    return spaces_.find(o[0]) != std::string::npos ? 1 : 0;
  };
  int is_unary_operator(const std::string &exact_operator_name) {
    return unary_operators_.count(exact_operator_name)
               ? exact_operator_name.size()
               : 0;
  };
  std::pair<int, token_type> is_token(const std::string &o);

  double get_operand(const std::string &s) { return std::atof(s.data()); };
  const TokenData &get_data(std::string s, token_type t) {
    return t == UNARYOPERATOR ? unary_operators_[s] : operators_[s];
  };

 protected:
  void count_length() {
    for (auto i : operators_) length_.insert(i.first.size());
  };

  std::set<int> length_;
  std::string spaces_ = " \t\n\r";
  std::string operand_mask_ = "([0-9]*[.,]?[0-9]+(?:[eE][-+]?[0-9]+)?).*";
  std::map<std::string, TokenData> unary_operators_{
      {"+", {UNARYOPERATOR, UOP_SCORE, [](double a, double) { return a; }}},
      {"-", {UNARYOPERATOR, UOP_SCORE, [](double a, double) { return -a; }}}};
  std::map<std::string, TokenData> operators_{
      {"+", {OPERATOR, ADD_SCORE, [](double a, double b) { return a + b; }}},
      {"-", {OPERATOR, SUB_SCORE, [](double a, double b) { return a - b; }}},
      {"/",
       {OPERATOR, DIV_SCORE,
        [](double a, double b) { return (b != 0) ? a / b : NAN; }}},
      {"*", {OPERATOR, MUL_SCORE, [](double a, double b) { return a * b; }}},
      {"%",
       {OPERATOR, DIV_SCORE, [](double a, double b) { return fmod(a, b); }}},
      {"mod",
       {OPERATOR, DIV_SCORE, [](double a, double b) { return fmod(a, b); }}},
      {"^",
       {OPERATOR, EXP_SCORE, [](double a, double b) { return pow(a, b); }}},
      // map<std::string, TokenData> functions_{
      {"abs", {FUNCTION, FUN_SCORE, [](double a, double) { return fabs(a); }}},
      {"sin", {FUNCTION, FUN_SCORE, [](double a, double) { return sin(a); }}},
      {"cos", {FUNCTION, FUN_SCORE, [](double a, double) { return cos(a); }}},
      {"tan", {FUNCTION, FUN_SCORE, [](double a, double) { return tan(a); }}},
      {"log", {FUNCTION, FUN_SCORE, [](double a, double) { return log10(a); }}},
      {"ln", {FUNCTION, FUN_SCORE, [](double a, double) { return log(a); }}},
      {"sqrt", {FUNCTION, FUN_SCORE, [](double a, double) { return sqrt(a); }}},
      {"asin", {FUNCTION, FUN_SCORE, [](double a, double) { return asin(a); }}},
      {"acos", {FUNCTION, FUN_SCORE, [](double a, double) { return acos(a); }}},
      {"atan", {FUNCTION, FUN_SCORE, [](double a, double) { return atan(a); }}},
      // Variables
      {"x", {VARIABLE, NO_SCORE, nullptr}},
      {"X", {VARIABLE, NO_SCORE, nullptr}},
      // Brackets
      {"(", {L_BRACKET, L_SCORE, nullptr}},
      {")", {R_BRACKET, R_SCORE, nullptr}},

  };
};

class TokenList : public std::list<ExprToken *> {
 public:
  using std::list<ExprToken *>::list;
  explicit TokenList(ExprSyntax *s) {
    syntax = s;
    brackets = 0;
  };
  ~TokenList() { clear_and_delete(); };
  void clear_and_delete() {
    for (auto t : *this) delete t;
    list::clear();
  };
  void make_infix_list(const std::string &str);

 protected:
  ExprToken *create_token(const std::string &str, token_type t);
  std::pair<int, token_type> find_token(const std::string &str);
  int skip_spaces(const std::string &str);
  void make_unary_operator();
  bool check_syntax(bool);
  ExprToken *before_back() {
    return (back() == front()) ? nullptr : *(prev(end(), 2));
  };
  int brackets = 0;
  ExprSyntax *syntax;
};
}  // namespace s21
#endif  // SRC_RPR_EXPR_H_
