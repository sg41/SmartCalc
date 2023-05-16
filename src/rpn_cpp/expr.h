#ifndef _SRC_RPN_CPP_EXPR_H_
#define _SRC_RPN_CPP_EXPR_H_

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

enum TokenType {
  kOperandToken,
  kOperatorToken,
  kUnaryOperatorToken,
  kFunctionToken,
  kVariableToken,
  kLBracketToken,
  kRBracketToken,
  kErrorToken
};

enum Precedence {
  kNoScore = 0,
  kAddScore = 2,
  kSubScore = 2,
  kMulScore = 4,
  kDivScore = 4,
  kFunctionScore = 10,
  kExponentScore = 6,
  kUnaryOperatorScore = 8,
  kLBracketScore = 12,
  kRBracketScore = 12,
};

using FuncType = double (*)(double, double);

typedef struct {
 public:
  TokenType t;
  Precedence p;
  FuncType call;
} TokenData;

class ExprToken {
 public:
  ExprToken(){};
  virtual ~ExprToken(){};
  explicit ExprToken(TokenType s, double d) : state_(s), data_(d){};
  virtual double &data() { return data_; };
  TokenType state() const { return state_; };
  void setState(TokenType s) { state_ = s; };
  virtual double func(double, double) { return data_; };
  virtual Precedence priority() { return kAddScore; };
  virtual std::string name() { return std::to_string(data_); };

 protected:
  TokenType state_ = kErrorToken;
  double data_ = 0;
};

class VarExprToken : public ExprToken {
 public:
  using ExprToken::ExprToken;
  VarExprToken(TokenType s, const std::string &n)
      : ExprToken(s, 0.0), var_name_(n){};
  std::string name() override { return var_name_; };

 protected:
  double *var_ref_ = nullptr;
  std::string var_name_ = "x";
};

class FuncExprToken : public ExprToken {
 public:
  using ExprToken::ExprToken;
  FuncExprToken(const std::string &n, TokenType s, Precedence p, FuncType f)
      : ExprToken(s, 0.0), priority_(p), fnc_(f), name_(n){};
  FuncExprToken(char c_name, TokenType s, Precedence p, FuncType f)
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
  Precedence priority() override { return priority_; };
  std::string name() override { return name_; };

 protected:
  Precedence priority_ = kAddScore;
  FuncType fnc_ = nullptr;
  std::string name_;
};

class ExprSyntax {
 public:
  ExprSyntax() { countLength(); };
  int isOperand(const std::string &o);
  int isSpace(char o) { return spaces_.find(o) != std::string::npos ? 1 : 0; };
  int isSpace(const std::string &o) {
    return spaces_.find(o[0]) != std::string::npos ? 1 : 0;
  };
  int isUnaryOperator(const std::string &exact_operator_name) {
    return unary_operators_.count(exact_operator_name)
               ? exact_operator_name.size()
               : 0;
  };
  std::pair<int, TokenType> isToken(const std::string &o);

  double getOperand(const std::string &s) { return std::atof(s.data()); };
  const TokenData &getData(std::string s, TokenType t) {
    return t == kUnaryOperatorToken ? unary_operators_[s] : operators_[s];
  };

 protected:
  void countLength() {
    for (auto i : operators_) length_.insert(i.first.size());
  };

  std::set<int> length_;
  std::string spaces_ = " \t\n\r";
  std::string operand_mask_ = "([0-9]*[.,]?[0-9]+(?:[eE][-+]?[0-9]+)?).*";
  std::map<std::string, TokenData> unary_operators_{
      {"+",
       {kUnaryOperatorToken, kUnaryOperatorScore,
        [](double a, double) { return a; }}},
      {"-", {kUnaryOperatorToken, kUnaryOperatorScore, [](double a, double) {
               return -a;
             }}}};
  std::map<std::string, TokenData> operators_{
      {"+",
       {kOperatorToken, kAddScore, [](double a, double b) { return a + b; }}},
      {"-",
       {kOperatorToken, kSubScore, [](double a, double b) { return a - b; }}},
      {"/",
       {kOperatorToken, kDivScore,
        [](double a, double b) { return (b != 0) ? a / b : NAN; }}},
      {"*",
       {kOperatorToken, kMulScore, [](double a, double b) { return a * b; }}},
      {"%",
       {kOperatorToken, kDivScore,
        [](double a, double b) { return fmod(a, b); }}},
      {"mod",
       {kOperatorToken, kDivScore,
        [](double a, double b) { return fmod(a, b); }}},
      {"^",
       {kOperatorToken, kExponentScore,
        [](double a, double b) { return pow(a, b); }}},
      // map<std::string, TokenData> functions_{
      {"abs",
       {kFunctionToken, kFunctionScore,
        [](double a, double) { return fabs(a); }}},
      {"sin",
       {kFunctionToken, kFunctionScore,
        [](double a, double) { return sin(a); }}},
      {"cos",
       {kFunctionToken, kFunctionScore,
        [](double a, double) { return cos(a); }}},
      {"tan",
       {kFunctionToken, kFunctionScore,
        [](double a, double) { return tan(a); }}},
      {"log",
       {kFunctionToken, kFunctionScore,
        [](double a, double) { return log10(a); }}},
      {"ln",
       {kFunctionToken, kFunctionScore,
        [](double a, double) { return log(a); }}},
      {"sqrt",
       {kFunctionToken, kFunctionScore,
        [](double a, double) { return sqrt(a); }}},
      {"asin",
       {kFunctionToken, kFunctionScore,
        [](double a, double) { return asin(a); }}},
      {"acos",
       {kFunctionToken, kFunctionScore,
        [](double a, double) { return acos(a); }}},
      {"atan",
       {kFunctionToken, kFunctionScore,
        [](double a, double) { return atan(a); }}},
      // Variables
      {"x", {kVariableToken, kNoScore, nullptr}},
      {"X", {kVariableToken, kNoScore, nullptr}},
      // Brackets
      {"(", {kLBracketToken, kLBracketScore, nullptr}},
      {")", {kRBracketToken, kRBracketScore, nullptr}},

  };
};

class TokenList : public std::list<ExprToken *> {
 public:
  using std::list<ExprToken *>::list;
  explicit TokenList(ExprSyntax *s) {
    syntax_ = s;
    brackets_ = 0;
  };
  ~TokenList() { clearAndDelete(); };
  void clearAndDelete() {
    for (auto t : *this) delete t;
    list::clear();
  };
  void makeInfixList(const std::string &str);

 protected:
  ExprToken *createToken(const std::string &str, TokenType t);
  std::pair<int, TokenType> findToken(const std::string &str);
  int skipSpaces(const std::string &str);
  void makeUnaryOperator();
  bool checkSyntax(bool);
  ExprToken *beforeBack() {
    return (back() == front()) ? nullptr : *(prev(end(), 2));
  };
  int brackets_ = 0;
  ExprSyntax *syntax_;
};
}  // namespace s21
#endif  // _SRC_RPN_CPP_EXPR_H_
