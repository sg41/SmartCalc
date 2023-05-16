#include <gtest/gtest.h>

#include "../graph.h"
#include "../rpr/core.h"
#include "../rpr/expr.h"
#include "../rpr/llist.h"
#include "../rpr/stack.h"
#define kEpsilon 1e-6
#define MIN_X -1000000
#define MAX_X 1000000
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

uint count_brackets(char *str) {
  uint res = 0;
  while (*str) {
    if (*str == '(' || *str == ')') res++;
    str++;
  }
  return res;
}

/*
 * LIST TESTS
 */

TEST(CalcTest, ll_list_1) {
  struct ll_node *l = ll_node_new(1, 2.2);
  uint expected_result, actual_result;
  expected_result = 1;
  actual_result = l->state;
  ASSERT_EQ(expected_result, actual_result);
  ASSERT_TRUE(fabs(2.2 - l->datum) < kEpsilon);
  ll_destroy_list(l);
}

TEST(CalcTest, ll_list_2) {
  struct ll_node *l = ll_node_new(1, 2.2);
  uint expected_result, actual_result;
  l = ll_node_prepend(l, 55, -99.99);
  expected_result = 55;
  actual_result = l->state;
  ASSERT_EQ(expected_result, actual_result);
  ASSERT_TRUE(fabs(-99.99 - l->datum) < kEpsilon);
  ll_destroy_list(l);
}

TEST(CalcTest, ll_list_3) {
  struct ll_node *l = ll_node_new(1, 2.2);
  uint expected_result, actual_result;
  ll_node_append(l, 55, -99.99);
  expected_result = 1;
  actual_result = l->state;
  ASSERT_EQ(expected_result, actual_result);
  ASSERT_EQ(l->next->state, 55);
  ASSERT_TRUE(fabs(2.2 - l->datum) < kEpsilon);
  ASSERT_TRUE(fabs(-99.99 - l->next->datum) < kEpsilon);
  ll_destroy_list(l);
}

TEST(CalcTest, ll_list_4) {
  struct ll_node *l = ll_node_new(1, 2.2);
  uint expected_result, actual_result;
  expected_result = 1;
  actual_result = l->state;
  ASSERT_EQ(expected_result, actual_result);
  ASSERT_TRUE(fabs(2.2 - l->datum) < kEpsilon);
  ll_node_destroy(&l);
}

TEST(CalcTest, ll_list_5) {
  struct ll_node *l = ll_node_new(1, 2.2);
  uint expected_result, actual_result;
  for (uint i = 0; i < 1000; i++) {
    ll_node_append(l, i, -99.99 + i);
  }
  expected_result = 999;
  actual_result = ll_last_node(l)->state;
  ASSERT_EQ(expected_result, actual_result);
  ASSERT_EQ(ll_last_node(l)->next, nullptr);
  ll_destroy_list(l);
}

TEST(CalcTest, ll_list_6) {
  struct ll_node *l = ll_node_new(1, 2.2);
  uint expected_result, actual_result;
  for (uint i = 0; i < 1000; i++) {
    ll_node_append(l, i, -99.99 + i);
  }
  expected_result = 998;
  actual_result = ll_before_last_node(l)->state;
  ASSERT_EQ(expected_result, actual_result);
  ASSERT_EQ(ll_before_last_node(l)->next, ll_last_node(l));
  ll_destroy_list(l);

  /*
   * STACK TESTS
   *
  struct stk *stk_new(void);
  void stk_destroy(struct stk **k);
  void stk_push(struct stk *k, const unsigned int s, const double d);
  void stk_pop(struct stk *k);
  double stk_peek(const struct stk *k);
  int stk_peek_status(const struct stk *k);
  */
}

TEST(CalcTest, stack_1) {
  struct stk *s = stk_new();
  uint expected_result, actual_result;
  expected_result = 0;
  actual_result = s->depth;
  ASSERT_EQ(expected_result, actual_result);
  ASSERT_EQ(s->top, nullptr);
  stk_destroy(&s);
}

TEST(CalcTest, stack_2) {
  struct stk *s = stk_new();
  uint expected_result, actual_result;
  stk_push(s, 7, -5.45);
  expected_result = 1;
  actual_result = s->depth;
  ASSERT_EQ(expected_result, actual_result);
  ASSERT_NE(s->top, nullptr);
  stk_destroy(&s);
}

TEST(CalcTest, stack_3) {
  struct stk *s = stk_new();
  uint expected_result, actual_result;
  stk_push(s, 7, -5.45);
  stk_pop(s);
  expected_result = 0;
  actual_result = s->depth;
  ASSERT_EQ(expected_result, actual_result);
  ASSERT_EQ(s->top, nullptr);
  stk_destroy(&s);
}

TEST(CalcTest, stack_4) {
  struct stk *s = stk_new();
  uint expected_result, actual_result;
  stk_push(s, 7, -5.45);
  expected_result = 7;
  actual_result = stk_peek_status(s);
  ASSERT_EQ(expected_result, actual_result);
  ASSERT_NE(s->top, nullptr);
  stk_destroy(&s);
}

TEST(CalcTest, stack_5) {
  struct stk *s = stk_new();
  uint expected_result, actual_result;
  stk_push(s, 7, -5.45);
  expected_result = 7;
  actual_result = stk_peek_status(s);
  ASSERT_EQ(expected_result, actual_result);
  ASSERT_NE(s->top, nullptr);
  ASSERT_TRUE(fabs(-5.45 - stk_peek(s)) < kEpsilon);
  stk_destroy(&s);

  /*
   * EXPR TESTS
   *
  struct expr *expr_new(void);
  void expr_destroy(struct expr **e);
  void expr_add_symbol(struct expr *e, const unsigned int s, const double d);
  struct expr *expr_shunt(const struct expr *e);
  char *one_expr_from_string(char *prog, struct expr **infix_to_fill, int *good,
                             int *parents);
  struct expr *expr_from_string(char *a, int *good);
  */
}

TEST(CalcTest, expr_1) {
  struct expr *e = expr_new();
  uint expected_result, actual_result;
  expected_result = 0;
  actual_result = e->length;
  ASSERT_EQ(expected_result, actual_result);
  ASSERT_EQ(e->head, nullptr);
  expr_destroy(&e);
}

TEST(CalcTest, expr_2) {
  struct expr *e = expr_new();
  uint expected_result, actual_result;
  expr_add_symbol(e, 77, DBL_MAX);
  expected_result = 1;
  actual_result = e->length;
  ASSERT_NE(e->head, nullptr);
  ASSERT_EQ(expected_result, actual_result);
  ASSERT_EQ(e->head->state, 77);
  ASSERT_TRUE(fabs(DBL_MAX - e->head->datum) < kEpsilon);
  expr_destroy(&e);
}

TEST(CalcTest, expr_3) {
  struct expr *e = expr_new();
  uint expected_result, actual_result;
  for (int i = 0; i < 10000; i++) {
    expr_add_symbol(e, i, DBL_MAX);
  }
  expected_result = 10000;
  actual_result = e->length;
  ASSERT_NE(e->head, nullptr);
  ASSERT_EQ(expected_result, actual_result);
  ASSERT_EQ(e->head->state, 0);
  ASSERT_TRUE(fabs(DBL_MAX - e->head->datum) < kEpsilon);
  expr_destroy(&e);
}

TEST(CalcTest, expr_4) {
  char str[1000] = "sin(cos(x^2)^(1*-100))*x";
  struct expr *e;
  uint expected_result, actual_result;
  int res = 0;
  e = expr_from_string(str, &res);
  expected_result = 18;
  actual_result = e->length;
  ASSERT_EQ(expected_result, actual_result);
  ASSERT_EQ(res, 1);
  expr_destroy(&e);
}

TEST(CalcTest, expr_5) {
  char str[1000] = "sin(cos(x^2)^(1*-100))*x";
  struct expr *e, *r;
  uint expected_result, actual_result;
  int res = 0;
  e = expr_from_string(str, &res);
  expected_result = 18;
  actual_result = e->length;
  ASSERT_EQ(expected_result, actual_result);
  ASSERT_EQ(res, 1);
  r = expr_shunt(e);
  expected_result = 12;
  actual_result = r->length;
  ASSERT_EQ(expected_result, actual_result);
  expr_destroy(&r);
  expr_destroy(&e);
}

TEST(CalcTest, expr_6_45) {
  char str[1000] = "45/8";
  struct expr *e, *r;
  uint expected_result, actual_result;
  int res = 0;
  e = expr_from_string(str, &res);
  expected_result = 3;
  actual_result = e->length;
  D("%s\n", "!!!45!!!");
  ll_print(e->head);
  ASSERT_EQ(expected_result, actual_result);
  ASSERT_EQ(res, 1);
  r = expr_shunt(e);
  expected_result = 3;
  actual_result = r->length;
  ASSERT_EQ(expected_result, actual_result);
  expr_destroy(&r);
  expr_destroy(&e);
}

TEST(CalcTest, expr_7_40) {
  char str[1000] = "45+234*(55+40/8)";
  struct expr *e, *r;
  uint expected_result, actual_result;
  int res = 0;
  e = expr_from_string(str, &res);
  expected_result = 11;
  actual_result = e->length;
  D("%s\n", "!!!40!!!");
  ll_print(e->head);
  ASSERT_EQ(expected_result, actual_result);
  ASSERT_EQ(res, 1);
  r = expr_shunt(e);
  expected_result = 9;
  actual_result = r->length;
  ASSERT_EQ(expected_result, actual_result);
  expr_destroy(&r);
  expr_destroy(&e);
}

TEST(CalcTest, expr_many_functions) {
  char str[1000] = "sin(cos(x^2)^(1*-100))*x   *tan(X)+sqrt(x/2)-log(x mod 2)";
  struct expr *e, *r;
  uint expected_result, actual_result;
  int res = 0;
  e = expr_from_string(str, &res);
  expected_result = 37;
  actual_result = e->length;
  ASSERT_EQ(expected_result, actual_result);
  ASSERT_EQ(res, 1);
  r = expr_shunt(e);
  expected_result = 37 - 12;
  actual_result = r->length;
  ASSERT_EQ(expected_result, actual_result);
  expr_destroy(&e);
  expr_destroy(&r);
}

TEST(CalcTest, expr_wrong_formula) {
#define __N__ 21
  char str[__N__][1000] = {
      "sinus(X)",
      "sin(eX) ",
      "sinus(cosinus(x^^2)^^^^(++1-*-100))*x(         )*tangens(X)+sqrt("
      "-x/2)-log(x mod 2)",
      "",
      {0},
      "+-x*cos(((",
      "mod 4",
      "cos(-x-)",
      "2 modulus 3",
      "2 -mod 4",
      "sin)x(",
      ")X(",
      " ",
      "русский язык рулит",
      "tan(-nan)",
      "1.999x",
      "xsin(x)",
      "sin(x)1",
      "sin(x)x",
      "1cos(x)",
      "sin(x)cos(x)"};

  struct expr *e;
  uint expected_result[__N__] = {1, 3, 1, 1, 1, 2, 1, 6, 3, 3, 1,
                                 1, 1, 1, 4, 2, 2, 5, 5, 2, 5},
       actual_result;
  int res = 0;
  // expected_result[1] = 3;
  // expected_result[5] = 2;
  for (int i = 0; i < __N__; i++) {
    D("%d\n", i);
    e = expr_from_string(str[i], &res);
    actual_result = e->length;
    ASSERT_EQ(expected_result[i], actual_result);
    ASSERT_EQ(res, 0);
    expr_destroy(&e);
  }
#undef __N__
}

TEST(CalcTest, expr_right_formula) {
  char str[13][1000] = {
      "sin(X)",
      "cos(X)",
      "(6 + 4)*cos(3*x) + (4 - 6)*sin(3*x) + x*((-5)*sin(3*x) + (12 - "
      "5)*cos(3*x)) + (25*sin(3*x) + 25*cos(X)) ",
      "(-x/2)-log(x mod 2)",
      "x",
      "-.5555*(sin(x))",
      "-x*cos(x)",
      "x mod 4",
      "cos(-x)",
      "2 mod 3",
      "-2 mod -4",
      "sin(x)-cos(x)",
      "-(-X)"};

  struct expr *e;
  uint expected_result[13] = {4, 4, 72, 13, 1, 9, 7, 3, 5, 3, 5, 9, 5},
       actual_result;
  int res = 1;
  for (int i = 0; i < 13; i++) {
    D("%d\n", i);
    e = expr_from_string(str[i], &res);
    actual_result = e->length;
    if (i == 2) ll_print(e->head);
    ASSERT_EQ(expected_result[i], actual_result);
    ASSERT_EQ(res, 1);
    expr_destroy(&e);
  }
}

TEST(CalcTest, expr_shunt_arithmetics) {
  char str[1000] = "x+1-x+x+x-x/2-x*0.01-x*2";
  struct expr *e, *r;
  uint expected_result, actual_result;
  int res = 0;
  e = expr_from_string(str, &res);
  expected_result = 21;
  actual_result = e->length;
  // D("%d\n", ll_print(e->head));
  ASSERT_EQ(expected_result, actual_result);
  ASSERT_EQ(res, 1);
  r = expr_shunt(e);
  // ll_print(r->head);
  expected_result = 21;
  actual_result = r->length;
  ASSERT_EQ(expected_result, actual_result);
  expr_destroy(&r);
  expr_destroy(&e);
}

TEST(CalcTest, expr_shunt_arithmetics_brackets) {
  char str[1000] = "x+1-x+(x+(x-x/2)-x*0.01-x*2)";
  struct expr *e, *r;
  uint expected_result, actual_result;
  int res = 0;
  e = expr_from_string(str, &res);
  expected_result = 25;
  actual_result = e->length;
  ASSERT_EQ(expected_result, actual_result);
  ASSERT_EQ(res, 1);
  r = expr_shunt(e);
  expected_result = 25 - 4;
  actual_result = r->length;
  ASSERT_EQ(expected_result, actual_result);
  expr_destroy(&r);
  expr_destroy(&e);
}

TEST(CalcTest, expr_shunt_right_formula) {
  char str[13][1000] = {
      "sin(X)",
      "cos(X)",
      "(6 + 4)*cos(3*x) + (4 - 6)*sin(3*x) + x*((-5)*sin(+3*x) + (12 - "
      "5)*cos(3*x)) + (25*sin(3*x) + 25*cos(X)) ",
      "(-x/2)-log(x mod 2)",
      "x",
      "-.5555*(sin(x))",
      "-x*cos(x)",
      "x mod 4",
      "cos(-x)",
      "2 mod 3",
      "-2 mod -4",
      "sin(x)-+cos(+x)",
      "-(-X)"};

  struct expr *e, *r;
  uint expected_result[13] = {4, 4, 73, 13, 1, 9, 7, 3, 5, 3, 5, 11, 5},
       actual_result;
  int res = 1;
  D("%s", "expr_shunt_right_formula\n");
  for (int i = 0; i < 13; i++) {
    D("%d\n", i);
    e = expr_from_string(str[i], &res);
    actual_result = e->length;
    if (i == 2) ll_print(e->head);
    ASSERT_EQ(expected_result[i], actual_result);
    ASSERT_EQ(res, 1);
    r = expr_shunt(e);
    actual_result = r->length;
    ASSERT_EQ(expected_result[i] - count_brackets(str[i]), actual_result);
    expr_destroy(&r);
    expr_destroy(&e);
  }
}

TEST(CalcTest, core_functions) {
  char str[10][1000] = {"sin(x)",  "cos(x)",  "tan(x)",  "log(x)",  "ln(x)",
                        "atan(x)", "asin(x)", "acos(x)", "sqrt(x)", "abs(x)"};
  typedef double (*f_ptr)(double x);
  f_ptr functions[10] = {
      sin, cos, tan, log10, log, atan, asin, acos, sqrt, fabs,
  };
  struct expr *e, *r;
  double expected_result, actual_result;
  int good = 0;
  for (int i = 0; i < 10; i++) {
    e = expr_from_string(str[i], &good);
    r = expr_shunt(e);
    D("i:%d func:%s\n", i, str[i]);
    for (double x = MIN_X; x <= MAX_X; x += 100) {
      actual_result = rpn_reduce(r, x);
      expected_result = functions[i](x);
      if (isfinite(expected_result)) {
        if (fabs(expected_result - actual_result) >= kEpsilon ||
            !isfinite(actual_result))
          D("e:%f a:%f\n", expected_result, actual_result);
        ASSERT_TRUE(fabs(expected_result - actual_result) < kEpsilon);
      } else {
        ASSERT_TRUE(!isfinite(actual_result));
      }
    }
    expr_destroy(&e);
    expr_destroy(&r);
  }
}

double ex1(double x) {
  return sin(pow(cos(pow(x, 2)), (1 * -100))) * x * tan(x) + sqrt(x / 2) -
         log(fmod(x, 2));
}
double ex2(double x) { return sin(pow(cos(pow(x, 2)), (1 * -100))) * x; }
double ex3(double x) {
  return pow(tan(x), (fabs(x))) / asin(pow(x, -1)) + 3.14 * log(x - x / 2);
}
double ex4(double x) {
  return x + 1 - x + (x + (x - x / 2) - x * 0.01 - x * 2);
}
double ex5(double x) {
  return (6 + 4) * cos(3 * x) + (4 - 6) * sin(3 * x) +
         x * ((-5) * sin(3 * x) + (12 - 5) * cos(3 * x)) +
         (25 * sin(3 * x) + 25 * cos(x));
}
double ex6(double x) { return 3 * pow(x, 2) + 1.5 * x + -.5; }
double ex7(double x) { return pow(sin(x / 2), 2) - (1 - cos(x)) / 2; }
double ex8(double x) { return sin(2 * x) / (1 + cos(2 * x)); }
double ex9(double x) {
  return sin(x) + cos(x) -
         fmod(tan(x) * log10(x) / pow(log(x), atan(x)),
              asin(x) / acos(x) * sqrt(x)) +
         fabs(x);
}
double ex10(double x) {
  return 3.14 * pow(sin(x), 2) + 3.14 / 2 * sin(x) * cos(x) +
         3.14 / 4 * pow(cos(x), 2);
}
double ex11(double x) {
  return 13 * cos(x) - 5 * cos(2 * x) - 2 * cos(3 * x) - cos(4 * x);
}
double ex12(double x) { return pow(-27, 1. / 3.) + x - x; }
TEST(CalcTest, core_random_expressions) {
#define __N__ 12
  char str[__N__][1000] = {
      "sin(cos(x^2)^(1*-100))*x   *tan(X)+sqrt(+x/2)-log(x mod 2)",
      "sin(cos(x^2)^(1*-100))*x",
      "tan(x)^(abs(x))/asin(x^-1)+3.14*ln(x-x/2)",
      "x+1-x+(x+(x-x/2)-x*0.01-x*2)",
      "(6 + 4)*cos(3*x) + (4 - 6)*sin(3*x) + x*((-5)*sin(3*x) + (12 - "
      "5)*cos(3*x)) + (25*sin(3*x) + 25*cos(X)) ",
      "3*x^2+1.5*x+-.5",
      "sin(x/2)^2-(1-cos(x))/2",
      "sin(2*x)/(1+cos(2*x))",
      "sin(x)+cos(x)-tan(x)*log(x)/ln(x)^atan(x) mod "
      "asin(x)/acos(x)*sqrt(x)+abs(x)",
      " 3.14*sin(x)^2+3.14/2*sin(x)*cos(x)+3.14/4*cos(+x)^2",
      "13*cos(x)-5*cos(2*x)-2*cos(3*x)-cos(4*x)",
      "-27^(1/3)"};
  typedef double (*f_ptr)(double x);
  f_ptr functions[__N__] = {ex1, ex2, ex3, ex4,  ex5,  ex6,
                            ex7, ex8, ex9, ex10, ex11, ex12};
  struct expr *e, *r;
  double expected_result, actual_result;
  int good = 0;
  for (int i = 0; i < __N__; i++) {
    D("i:%d func:%s\n", i, str[i]);
    e = expr_from_string(str[i], &good);
    r = expr_shunt(e);
    D("GOOD:%d \n", good);
    for (double x = MIN_X; x <= MAX_X; x += 100) {
      actual_result = rpn_reduce(r, x);
      expected_result = functions[i](x);
      if (isfinite(expected_result)) {
        if (fabs(expected_result - actual_result) >= kEpsilon ||
            !isfinite(actual_result))
          D("e:%.7f a:%.7f\n", expected_result, actual_result);
        ASSERT_TRUE(fabs(expected_result - actual_result) < kEpsilon);
      } else {
        ASSERT_TRUE(!isfinite(actual_result));
      }
    }
    expr_destroy(&r);
    expr_destroy(&e);
  }
#undef __N__
}

namespace test2 {
double ex1(double x) {
  return sin(pow(cos(pow(x, 2)), (1 * -100))) * x * tan(x) + sqrt(x / 2) -
         log(fmod(x, 2));
}
double ex2(double x) { return sin(pow(cos(pow(x, 2)), (1 * -100))) * x; }
double ex3(double x) {
  return pow(tan(x), (fabs(x))) / asin(pow(x, -1)) + 3.14 * log(x - x / 2);
}
double ex4(double x) {
  return x + 1 - x + (x + (x - x / 2) - x * 0.01 - x * 2);
}
double ex5(double x) {
  return (6 + 4) * cos(3 * x) + (4 - 6) * sin(3 * x) +
         x * ((-5) * sin(3 * x) + (12 - 5) * cos(3 * x)) +
         (25 * sin(3 * x) + 25 * cos(x));
}
double ex6(double x) { return 3 * pow(x, 2) + 1.5 * x + -.5; }
double ex7(double x) { return pow(sin(x / 2), 2) - (1 - cos(x)) / 2; }
double ex8(double x) { return sin(2 * x) / (1 + cos(2 * x)); }
double ex9(double x) {
  return sin(x) + cos(x) -
         fmod(tan(x) * log10(x) / pow(log(x), atan(x)),
              asin(x) / acos(x) * sqrt(x)) +
         fabs(x);
}
double ex10(double x) {
  return 3.14 * pow(sin(x), 2) + 3.14 / 2 * sin(x) * cos(x) +
         3.14 / 4 * pow(cos(x), 2);
}
double ex11(double x) {
  return 13 * cos(x) - 5 * cos(2 * x) - 2 * cos(3 * x) - cos(4 * x);
}
double ex12(double x) {
  return 100000 * (9.5 / 100 / 12 * pow(9.5 / 100 / 12 + 1, 12) /
                   (pow(1 + 9.5 / 100 / 12, 12) - 1)) +
         (x - x);
}
TEST(CalcTest, core_calc) {
  char str[12][1000] = {
      "sin(cos(x^2)^(1*-100))*x   *tan(X)+sqrt(+x/2)-log(x mod 2)",
      "sin(cos(x^2)^(1*-100))*x",
      "tan(x)^(abs(x))/asin(x^-1)+3.14*ln(x-x/2)",
      "x+1-x+(x+(x-x/2)-x*0.01-x*2)",
      "(6 + 4)*cos(3*x) + (4 - 6)*sin(3*x) + x*((-5)*sin(3*x) + (12 - "
      "5)*cos(3*x)) + (25*sin(3*x) + 25*cos(X)) ",
      "3*x^2+1.5*x+-.5",
      "sin(x/2)^2-(1-cos(x))/2",
      "sin(2*x)/(1+cos(2*x))",
      "sin(x)+cos(x)-tan(x)*log(x)/ln(x)^atan(x) mod "
      "asin(x)/acos(x)*sqrt(x)+abs(x)",
      " 3.14*sin(x)^2+3.14/2*sin(x)*cos(x)+3.14/4*cos(+x)^2",
      "13*cos(x)-5*cos(2*x)-2*cos(3*x)-cos(4*x)",
      "100000*(9.5/100/12*(9.5/100/12+1)^12/((1+9.5/100/12)^12-1))"};
  typedef double (*f_ptr)(double x);
  f_ptr functions[12] = {ex1, ex2, ex3, ex4,  ex5,  ex6,
                         ex7, ex8, ex9, ex10, ex11, ex12};
  double expected_result, actual_result;
  int good = 0;
  for (int i = 0; i < 12; i++) {
    D("i:%d func:%s\n", i, str[i]);
    D("GOOD:%d \n", good);
    for (double x = MIN_X; x <= MAX_X; x += 100) {
      actual_result = calc(str[i], x, &good);
      expected_result = functions[i](x);
      if (isfinite(expected_result)) {
        if (fabs(expected_result - actual_result) >= kEpsilon ||
            !isfinite(actual_result))
          D("e:%.7f a:%.7f\n", expected_result, actual_result);
        ASSERT_TRUE(fabs(expected_result - actual_result) < kEpsilon);
      } else {
        ASSERT_TRUE(!isfinite(actual_result));
      }
    }
  }
}
}  // namespace test2
TEST(CalcTest, core_credit_calc) {
  int good;
  double expected_result =
      100000 * (9.5 / 100 / 12 * pow(9.5 / 100 / 12 + 1, 12) /
                (pow(1 + 9.5 / 100 / 12, 12) - 1));
  double actual_result = calc(
      (char *)"100000*(9.5/100/12*(9.5/100/12+1)^12/((1+9.5/100/12)^12-1))", 0,
      &good);
  if (fabs(expected_result - actual_result) > kEpsilon)
    D("e:%.7f a:%.7f\n", expected_result, actual_result);
  ASSERT_TRUE(fabs(expected_result - actual_result) < kEpsilon);
}

TEST(CalcTest, core_calc_nan) {
  int good;
  int expected_result = 0;
  calc((char *)"tan(-nan)", 0, &good);
  ASSERT_EQ(expected_result, good);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
