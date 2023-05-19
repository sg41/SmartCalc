#include "expr.h"

#include <memory>

using namespace s21;

int ExprSyntax::isOperand(const std::string &o) {
  int res = 0;
  static const std::regex base_regex(operand_mask_);
  std::smatch base_match;
  if (std::regex_match(o, base_match, base_regex)) {
    res = base_match[1].str().size();
  }
  return res;
};

std::pair<int, TokenType> ExprSyntax::isToken(const std::string &o) {
  int res = 0, len = 0;
  auto l = length_.rbegin();
  for (; res == 0 && l != length_.rend(); l++) {
    len = *l;
    res = operators_.count(o.substr(0, len));
  }
  return res == 0 ? std::make_pair(0, kErrorToken)
                  : std::make_pair(len, operators_[o.substr(0, len)].t);
};

void TokenList::makeUnaryOperator() {
  ExprToken *last = back();
  ExprToken *before = beforeBack();

  if ((last->state() == kOperatorToken &&
       syntax_->isUnaryOperator(last->name())) &&
      (before == nullptr || ((before->state() == kOperatorToken) ||
                             (before->state() == kLBracketToken)))) {
    pop_back();
    push_back(new FuncExprToken(
        last->name(), syntax_->getData(last->name(), kUnaryOperatorToken)));
    delete last;
  }
}

bool TokenList::checkSyntax(bool last_check = false) {
  ExprToken *last = back();
  ExprToken *before = beforeBack();

  bool good = true;
  if (before != nullptr) {
    if (last->state() == kErrorToken) good = false;
    if (before->state() == kUnaryOperatorToken &&
        (last->state() == kOperatorToken || last->state() == kRBracketToken))
      good = false;

    if (last->state() == before->state() && last->state() != kLBracketToken &&
        last->state() != kRBracketToken)
      good = false;

    if (before->state() == kLBracketToken && last->state() == kRBracketToken)
      good = false;
    if (before->state() == kOperatorToken && last->state() == kRBracketToken)
      good = false;
    if (before->state() == kRBracketToken &&
        (last->state() != kOperatorToken && last->state() != kRBracketToken))
      good = false;

    if (before->state() == kOperandToken &&
        (last->state() == kVariableToken || last->state() == kLBracketToken))
      good = false;
    if (before->state() == kVariableToken &&
        (last->state() == kOperandToken || last->state() == kLBracketToken))
      good = false;

    if (before->state() != kOperatorToken &&
        before->state() != kUnaryOperatorToken &&
        before->state() != kLBracketToken && last->state() == kFunctionToken)
      good = false;
  } else {
    if (last->state() == kOperatorToken) good = false;
  }
  if (last_check &&
      (last->state() == kOperatorToken ||
       last->state() == kUnaryOperatorToken ||
       last->state() == kFunctionToken || last->state() == kLBracketToken))
    good = false;
  if (brackets_ < 0) good = false;
  return good;
};

int TokenList::skipSpaces(const std::string &str) {
  int i = 0;
  while (static_cast<size_t>(i) < str.size() && syntax_->isSpace(str[i]))
    i++;  // Skip spaces
  return i;
};

std::pair<int, TokenType> TokenList::findToken(const std::string &str) {
  int l = syntax_->isOperand(str);
  TokenType t = kOperandToken;
  return l != 0 ? std::make_pair(l, t) : syntax_->isToken(str);
};

ExprToken *TokenList::createToken(const std::string &str_token, TokenType t) {
  ExprToken *token;

  if (t == kOperatorToken || t == kUnaryOperatorToken || t == kFunctionToken ||
      t == kLBracketToken || t == kRBracketToken)
    token = new FuncExprToken(str_token, syntax_->getData(str_token, t));
  if (t == kLBracketToken) {
    brackets_++;
  } else if (t == kRBracketToken) {
    brackets_--;
  }
  if (t == kOperandToken) {
    token = new ExprToken(t, syntax_->getOperand(str_token));
  }
  if (t == kVariableToken) {
    token = new VarExprToken(t, str_token);
  }

  return token;
};

void TokenList::makeInfixList(const std::string &s) {
  if (!empty()) clearAndDelete();
  brackets_ = 0;
  bool good = true;
  if (s.size() != 0) {
    size_t i = 0;
    while (i < s.size() && good) {
      i += skipSpaces(s.substr(i));  // Skip spaces
      if (i < s.size()) {
        int l;
        TokenType t;
        std::tie(l, t) = findToken(s.substr(i));
        if (l) {
          push_back(createToken(s.substr(i, l), t));
          makeUnaryOperator();
          good = checkSyntax();
          i += l;
        } else {
          good = false;
        }
      }
    }
  } else {
    throw std::invalid_argument("Empty expression string");
  }

  if (empty()) throw std::invalid_argument("No valid expression found");
  if (good != false) good = checkSyntax(true);
  if (good == false) throw std::invalid_argument("Parsing kErrorToken");
  if (brackets_ != 0) throw std::invalid_argument("Wrong () pairs");
};
