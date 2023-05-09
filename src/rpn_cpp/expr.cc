#include "expr.h"

#include <memory>

using namespace s21;

int ExprSyntax::is_operand(const std::string &o) {
  int res = 0;
  static const std::regex base_regex(operand_mask_);
  std::smatch base_match;
  if (std::regex_match(o, base_match, base_regex)) {
    res = base_match[1].str().size();
  }
  return res;
};

std::pair<int, token_type> ExprSyntax::is_token(const std::string &o) {
  int res = 0, len = 0;
  auto l = length_.rbegin();
  for (; res == 0 && l != length_.rend(); l++) {
    len = *l;
    res = operators_.count(o.substr(0, len));
  }
  return res == 0 ? std::make_pair(0, ERROR)
                  : std::make_pair(len, operators_[o.substr(0, len)].t);
};

void TokenList::make_unary_operator() {
  ExprToken *last = back();
  ExprToken *before = before_back();

  if ((last->state() == OPERATOR && syntax->is_unary_operator(last->name())) &&
      (before == nullptr ||
       ((before->state() == OPERATOR) || (before->state() == L_BRACKET)))) {
    pop_back();
    push_back(new FuncExprToken(last->name(),
                                syntax->get_data(last->name(), UNARYOPERATOR)));
    delete last;
  }
}

bool TokenList::check_syntax(bool last_check = false) {
  ExprToken *last = back();
  ExprToken *before = before_back();

  bool good = true;
  if (before != nullptr) {
    if (last->state() == ERROR) good = false;
    if (before->state() == UNARYOPERATOR &&
        (last->state() == OPERATOR || last->state() == R_BRACKET))
      good = false;

    if (last->state() == before->state() && last->state() != L_BRACKET &&
        last->state() != R_BRACKET)
      good = false;

    if (before->state() == L_BRACKET && last->state() == R_BRACKET)
      good = false;
    if (before->state() == OPERATOR && last->state() == R_BRACKET) good = false;
    if (before->state() == R_BRACKET &&
        (last->state() != OPERATOR && last->state() != R_BRACKET))
      good = false;

    if (before->state() == OPERAND &&
        (last->state() == VARIABLE || last->state() == L_BRACKET))
      good = false;
    if (before->state() == VARIABLE &&
        (last->state() == OPERAND || last->state() == L_BRACKET))
      good = false;

    if (before->state() != OPERATOR && before->state() != UNARYOPERATOR &&
        before->state() != L_BRACKET && last->state() == FUNCTION)
      good = false;
  } else {
    if (last->state() == OPERATOR) good = false;
  }
  if (last_check &&
      (last->state() == OPERATOR || last->state() == UNARYOPERATOR ||
       last->state() == FUNCTION || last->state() == L_BRACKET))
    good = false;
  if (brackets < 0) good = false;
  return good;
};

int TokenList::skip_spaces(const std::string &str) {
  int i = 0;
  while (static_cast<size_t>(i) < str.size() && syntax->is_space(str[i]))
    i++;  // Skip spaces
  return i;
};

std::pair<int, token_type> TokenList::find_token(const std::string &str) {
  int l = syntax->is_operand(str);
  token_type t = OPERAND;
  return l != 0 ? std::make_pair(l, t) : syntax->is_token(str);
};

ExprToken *TokenList::create_token(const std::string &str_token, token_type t) {
  ExprToken *token;

  if (t == OPERATOR || t == UNARYOPERATOR || t == FUNCTION || t == L_BRACKET ||
      t == R_BRACKET)
    token = new FuncExprToken(str_token, syntax->get_data(str_token, t));
  if (t == L_BRACKET) {
    brackets++;
  } else if ((t == R_BRACKET)) {
    brackets--;
  }
  if ((t == OPERAND)) {
    token = new ExprToken(t, syntax->get_operand(str_token));
  }
  if ((t == VARIABLE)) {
    token = new VarExprToken(t, str_token);
  }

  return token;
};

void TokenList::make_infix_list(const std::string &s) {
  if (!empty()) clear_and_delete();
  brackets = 0;
  bool good = true;
  if (s.size() != 0) {
    size_t i = 0;
    while (i < s.size() && good) {
      i += skip_spaces(s.substr(i));  // Skip spaces
      if (i < s.size()) {
        int l;
        token_type t;
        std::tie(l, t) = find_token(s.substr(i));
        if (l) {
          push_back(create_token(s.substr(i, l), t));
          make_unary_operator();
          good = check_syntax();
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
  if (good != false) good = check_syntax(true);
  if (good == false) throw std::invalid_argument("Parsing error");
  if (brackets != 0) throw std::invalid_argument("Wrong () pairs");
};
