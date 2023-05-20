#include <gtest/gtest.h>

#include "../models/creditmodel.h"
#include "../models/depositmodel.h"
#include "../models/graphmodel.h"
#include "../rpn_cpp/core.h"

using namespace s21;
class TestObserver : public ModelObserverInterface<GraphModelData> {
 public:
  void observerUpdate(const GraphModelData *d) override {
    if (d->x) {  /// Unused variable check error bypass
    }
#ifndef NDEBUG
    std::cout << "Updated" << std::endl;
    std::cout << d->clip_x1 << std::endl;
#endif
  };
};
static const int kTestMinX = -1000000;
static const int kTestMaxX = 1000000;
static const int kTestFinanceEpsilon = 1;

TEST(CalcTest, model) {
  TestObserver o;
  GraphModel m1;
#ifndef NDEBUG
  int i = 0;
#endif
  m1.registerObserver(&o);
  m1.notifyObservers();
  m1.removeObserver(&o);
  m1.notifyObservers();
  DBPRINT("%d\n", i++);
  GraphModelData data = *(m1.getData());
  GraphModelData *d = &data;
  EXPECT_EQ(m1.validateData(), 0);
  DBPRINT("%d\n", i++);
  d->max_x = GraphModelData::kVeryMaxX + 10;
  d->str = "tan(X)";
  EXPECT_GT(d->max_x, GraphModelData::kVeryMaxX + 0);
  m1.setData(d);
  DBPRINT("%d\n", i++);
  EXPECT_NE(m1.validateData(), 0);
  GraphModel m2(m1);
  GraphModelData data2 = *(m2.getData());
  GraphModelData *d2 = &data2;
  DBPRINT("%d\n", i++);
  EXPECT_GT(d2->max_x, GraphModelData::kVeryMaxX + 0);
  d2->x = 999;
  EXPECT_NE(d->x, d2->x);
  m2.clearData();
  d2->initData();
  DBPRINT("%d\n", i++);
  d2->x = 1;
  d2->str = "sin(x)";
  m2.setData(d2);
  DBPRINT("%d\n", i++);
  EXPECT_NO_THROW(m2.calculate());
  DBPRINT("%d\n", i++);
  EXPECT_NEAR(m2.getData()->y, 0.841471, BaseCalcData::kEpsilon);
  DBPRINT("%d\n", i++);
  m1.clearData();
  m1 = m2;
  DBPRINT("%d\n", i++);
  EXPECT_EQ(m1.getData()->str, "sin(x)");
  m1 = GraphModel();
  DBPRINT("%d\n", i++);
  EXPECT_NE(m1.getData()->str, "sin(x)");
  m1 = std::move(m2);
  DBPRINT("%d\n", i++);
  EXPECT_EQ(m1.getData()->str, "sin(x)");
#ifndef __APPLE__
  GraphModel m3(std::move(GraphModel()));
  EXPECT_EQ(m3.getData()->min_y, -1);
#endif
}

TEST(CalcTest, model_tan_calc) {
  GraphModel m;
  GraphModelData d;
  d.str = "tan(X)";
  d.dx = 0.0001;
  m.setData(&d);
  m.calculate();
  bool res = false;
  for (auto n : m.getData()->y_vect) {
    if (isnan(n)) res = true;
  }
  EXPECT_TRUE(res);
}

TEST(CalcTest, model_1_x_calc) {
  GraphModel m;
  GraphModelData d;
  d.str = "X^x";
  d.dx = 0.0001;
  m.setData(&d);
  m.calculate();
  bool res = false;
  for (auto n : m.getData()->y_vect) {
    if (isinf(n)) res = true;
  }
  EXPECT_TRUE(res);
}

TEST(CalcTest, regex_5) {
  char str[1000] = "2.365sdsdsd";
  uint expected_result, actual_result;
  Syntax s;
  actual_result = s.isOperand(str);
  expected_result = 5;
  EXPECT_EQ(expected_result, actual_result);
  double a, b = 2.365;
  a = s.getOperand(str);
  EXPECT_EQ(a, b);
  b = 1.890e4;
  actual_result = s.isOperand("1.890e4fdj;alkf;");
  expected_result = 7;
  EXPECT_EQ(expected_result, actual_result);
  a = s.getOperand("1.890e4fdj;alkf;");
  EXPECT_EQ(a, b);
  b = 1.890E-4;
  actual_result = s.isOperand("1.890E-4aslkdjalks147981asfja08--2344");
  expected_result = 8;
  EXPECT_EQ(expected_result, actual_result);

  a = s.getOperand("1.890E-4aslkdjalks147981asfja08--2344");
  EXPECT_EQ(a, b);
}

TEST(CalcTest, expr_6) {
  char str[1000] = "sin( cos(x^2)^(1*-100))*x";
  uint expected_result, actual_result;
  Syntax s;
  TokenList infix(&s);
  infix.makeInfixList(str);
  expected_result = 18;
  actual_result = infix.size();
  EXPECT_EQ(expected_result, actual_result);
}

#ifdef NDEBUG
TEST(CalcTest, expr_wrong_formula) {
#define __N__ 28
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
      "sin(x)cos(x)",
      "23*",
      "*",
      "*21",
      "23*+",
      "sin(a)",
      "x2",
      "x()"};

  uint expected_result[__N__] = {1, 2, 1, 0, 0, 2, 1, 6, 2, 3, 2, 1, 0, 0,
                                 3, 2, 2, 5, 5, 2, 5, 2, 1, 1, 3, 2, 2, 2},
       actual_result;

  Syntax s;
  TokenList infix(&s);
  for (int i = 0; i < __N__; i++) {
    EXPECT_THROW(infix.makeInfixList(str[i]), std::invalid_argument);
    actual_result = infix.size();
    EXPECT_EQ(expected_result[i], actual_result);
  }
#undef __N__
}
#endif

TEST(CalcTest, expr_right_formula) {
  char str[13][1000] = {
      "sin(X)",
      "cos(X)",
      "(6 + 4)*cos(3*x) + (4 - 6)*sin(3*x) + x*((-5)*abs(3*x) + (12 - \
          5)*cos(3*x)) + (25*sin(3*x) + 25*cos(X)) ",
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

  uint expected_result[13] = {4, 4, 72, 13, 1, 9, 7, 3, 5, 3, 5, 9, 5},
       actual_result;

  for (int i = 0; i < 13; i++) {
    Syntax s;
    TokenList infix(&s);
    EXPECT_NO_THROW(infix.makeInfixList(str[i]));
    actual_result = infix.size();
    EXPECT_EQ(expected_result[i], actual_result);
  }
}

TEST(CalcTest, shunt_1) {
  char str[1000] = "sin(cos(x^2)^(1*-100))*x";
  CalcCore c;
  c.makeRpnExpr(str);
  int expected_result = 12;
  int actual_result = c.getExprSize();
  ASSERT_EQ(expected_result, actual_result);
}

TEST(CalcTest, shunt_2) {
  char str[1000] = "45/8";
  CalcCore c;
  c.makeRpnExpr(str);
  int expected_result = 3;
  int actual_result = c.getExprSize();
  ASSERT_EQ(expected_result, actual_result);
}

TEST(CalcTest, shunt_3) {
  char str[1000] = "45+234*(55+40/8)";
  CalcCore c;
  c.makeRpnExpr(str);
  int expected_result = 9;
  int actual_result = c.getExprSize();
  ASSERT_EQ(expected_result, actual_result);
}

TEST(CalcTest, shunt_4) {
  char str[1000] = "sin(cos(x^2)^(1*-100))*x   *tan(X)+sqrt(x/2)-log(x mod 2)";
  CalcCore c;
  c.makeRpnExpr(str);
  int expected_result = 37 - 12;
  int actual_result = c.getExprSize();
  ASSERT_EQ(expected_result, actual_result);
}

TEST(CalcTest, shunt_5) {
  char str[1000] = "x+1-x+x+x-x/2-x*0.01-x*2";
  CalcCore c;
  c.makeRpnExpr(str);
  int expected_result = 21;
  int actual_result = c.getExprSize();
  ASSERT_EQ(expected_result, actual_result);
}

TEST(CalcTest, shunt_6) {
  char str[1000] = "x+1-x+(x+(x-x/2)-x*0.01-x*2)";
  CalcCore c;
  c.makeRpnExpr(str);
  int expected_result = 21;
  int actual_result = c.getExprSize();
  ASSERT_EQ(expected_result, actual_result);
}

uint count_brackets(char *str) {
  uint res = 0;
  while (*str) {
    if (*str == '(' || *str == ')') res++;
    str++;
  }
  return res;
}

TEST(CalcTest, expr_shunt_right_formula) {
  char str[13][1000] = {
      "sin(X)",
      "cos(X)",
      "(6 + 4)*cos(3*x) + (4 - 6)*sin(3*x) + x*((-5)*abs(+3*x) + (12 - \
      5)*cos(3*x)) + (25*sin(3*x) + 25*cos(X)) ",
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

  uint const expected_result[13] = {4, 4, 73, 13, 1, 9, 7, 3, 5, 3, 5, 11, 5};
  uint actual_result;
  CalcCore c;
  for (int i = 0; i < 13; i++) {
    c.makeRpnExpr(str[i]);
    actual_result = c.getExprSize();
    EXPECT_EQ(expected_result[i] - count_brackets(str[i]), actual_result);
  }
}

TEST(CalcTest, core_functions) {
  char str[10][1000] = {"sin(x)",  "cos(x)",  "tan(x)",  "log(x)",  "ln(x)",
                        "atan(x)", "asin(x)", "acos(x)", "sqrt(x)", "abs(x)"};
  typedef double (*f_ptr)(double x);
  f_ptr functions[10] = {
      sin, cos, tan, log10, log, atan, asin, acos, sqrt, fabs,
  };
  double expected_result, actual_result;
  CalcCore c;

  for (int i = 0; i < 10; i++) {
    c.makeRpnExpr(str[i]);
    DBPRINT("i:%d func:%s\n", i, str[i]);
    for (double x = kTestMinX; x <= kTestMaxX; x += 100) {
      actual_result = c.calc(x);
      expected_result = functions[i](x);
      if (isfinite(expected_result)) {
        if (fabs(expected_result - actual_result) >= BaseCalcData::kEpsilon ||
            !isfinite(actual_result))
          DBPRINT("e:%f a:%f\n", expected_result, actual_result);
        ASSERT_TRUE(fabs(expected_result - actual_result) <
                    BaseCalcData::kEpsilon);
      } else {
        ASSERT_TRUE(!isfinite(actual_result));
      }
    }
  }
}

TEST(CalcTest, expr_calc) {
  char str[1000] = "sin(cos(x^2)^(1*-100))*x";
  CalcCore c;
  c.makeRpnExpr(str);
  EXPECT_TRUE(c.calc(1) - 0.7919175265 < BaseCalcData::kEpsilon);
}

TEST(CalcTest, exponent_calc) {
  char str[1000] = "x^2^3";
  CalcCore c;
  c.makeRpnExpr(str);
  double res = c.calc(2);
  c.makeRpnExpr("x^(2^3)");
  EXPECT_EQ(c.calc(2), res);
}

#ifdef NDEBUG
TEST(CalcTest, expr_calc_1) {
  char str[1000] = "sin()";
  CalcCore c;
  ASSERT_ANY_THROW(c.makeRpnExpr(str));
}
#endif

TEST(CalcTest, expr_data_1) {
  DataToken t;
  t.data() = 3;
  EXPECT_EQ(t.func(1.2, 2.3), 3);
  EXPECT_EQ(t.priority(), kAddScore);
  EXPECT_EQ(t.ass(), kLeftAss);
  EXPECT_EQ(t.name(), "3.000000");
  VariableToken v;
  EXPECT_EQ(v.name(), "x");
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
double ex13(double x) { return fabs(sin(fabs(x))); }
double ex14(double x) { return sin(cos(tan(x))); }
TEST(CalcTest, core_random_expressions) {
#define __N__ 14
  char str[__N__][1000] = {
      "sin(cos(x^2)^(1*-100))*x   *tan(X)+sqrt(+x/2)-log(x % 2)",
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
      "-27^(1/3)",
      "abs(sin(abs(x)))",
      "sin(cos(tan(x)))"};
  typedef double (*f_ptr)(double x);
  f_ptr functions[__N__] = {ex1, ex2, ex3,  ex4,  ex5,  ex6,  ex7,
                            ex8, ex9, ex10, ex11, ex12, ex13, ex14};
  double expected_result, actual_result;
  CalcCore c;
  for (int i = 0; i < __N__; i++) {
    c.makeRpnExpr(str[i]);
    DBPRINT("i:%d func:%s\n", i, str[i]);
    for (double x = kTestMinX; x <= kTestMaxX; x += 1000) {
      actual_result = c.calc(x);
      expected_result = functions[i](x);
      if (isfinite(expected_result)) {
        if (fabs(expected_result - actual_result) >= BaseCalcData::kEpsilon ||
            !isfinite(actual_result))
          DBPRINT("e:%f a:%f\n", expected_result, actual_result);
        ASSERT_TRUE(fabs(expected_result - actual_result) <
                    BaseCalcData::kEpsilon);
      } else {
        ASSERT_TRUE(!isfinite(actual_result));
      }
    }
  }
#undef __N__
}

TEST(CreditTest, ann) {
  CreditModel calc;
  CreditModelData data = *(calc.getData());
  CreditModelData *d = &data;
  d->type = d->kAnnuity;
  d->amount = 123456;
  d->duration = 120;
  d->rate = 4.56;
  calc.setData(d);
  calc.calculate();
  data = *calc.getData();
  ASSERT_NEAR(d->total_payment, 153966.00, BaseCalcData::kEpsilon);
  ASSERT_NEAR(d->overpayment, 30510.00, BaseCalcData::kEpsilon);
  ASSERT_NEAR(d->monthly_payment, 1283.05, BaseCalcData::kEpsilon);
  d->round = true;
  calc.setData(d);
  calc.calculate();
  data = *calc.getData();
  ASSERT_NEAR(d->monthly_payment, 1283, BaseCalcData::kEpsilon);
}

TEST(CreditTest, small) {
  CreditModel calc;
  CreditModelData data = *(calc.getData());
  CreditModelData *d = &data;
  d->type = d->kAnnuity;
  d->amount = 1;
  d->duration = 120;
  d->rate = 0.001;
  d->round = true;
  calc.setData(d);
  calc.calculate();
  data = *calc.getData();
  ASSERT_NEAR(d->error, 2, BaseCalcData::kEpsilon);
}

TEST(CreditTest, ann_banki) {
  CreditModel calc;
  CreditModelData data = *(calc.getData());
  CreditModelData *d = &data;
  d->type = d->kAnnuity;
  d->amount = 50000;
  d->duration = 6;
  d->rate = 14;
  d->round = true;
  calc.setData(d);
  calc.calculate();
  data = *calc.getData();
  ASSERT_NEAR(d->total_payment, 52062, BaseCalcData::kEpsilon);
  ASSERT_NEAR(d->overpayment, 2062, BaseCalcData::kEpsilon);
  ASSERT_NEAR(d->monthly_payment, 8677, BaseCalcData::kEpsilon);
}

TEST(CreditTest, diff) {
  CreditModel calc;
  CreditModelData data = *(calc.getData());
  CreditModelData *d = &data;
  d->type = d->kDifferentiated;
  d->amount = 100000;
  d->duration = 6;
  d->rate = 12.5;
  calc.setData(d);
  calc.calculate();
  data = *calc.getData();
  ASSERT_NEAR(d->total_payment, 103645.83, BaseCalcData::kEpsilon);
  ASSERT_NEAR(d->overpayment, 3645.83, BaseCalcData::kEpsilon);
  std::vector<double> expected_mp = {17708.33, 17534.72, 17361.11,
                                     17187.50, 17013.89, 16840.28};
  for (int i = 0; i < 6; ++i) {
    ASSERT_NEAR(d->monthly_payments[i], expected_mp.at(i),
                BaseCalcData::kEpsilon);
  }
  d->round = true;
  calc.setData(d);
  calc.calculate();
  data = *calc.getData();
  ASSERT_NEAR(d->total_payment, 103646, BaseCalcData::kEpsilon);
  ASSERT_NEAR(d->overpayment, 3646, BaseCalcData::kEpsilon);
}

TEST(CreditTest, setData) {
  CreditModel calc;
  CreditModelData d;
  d.type = d.kAnnuity;
  d.amount = 123456;
  d.duration = 120;
  d.rate = 4.56;
  calc.setData(&d);
  calc.calculate();
  d = *calc.getData();
  ASSERT_NEAR(d.total_payment, 153966.00, BaseCalcData::kEpsilon);
  ASSERT_NEAR(d.overpayment, 30510.00, BaseCalcData::kEpsilon);
  ASSERT_NEAR(d.monthly_payment, 1283.05, BaseCalcData::kEpsilon);
  d.round = true;
  calc.setData(&d);
  calc.calculate();
  d = *calc.getData();
  ASSERT_NEAR(d.monthly_payment, 1283, BaseCalcData::kEpsilon);
}

TEST(DepositTest, no_cap) {
  DepositModel calc;
  DepositModelData d;
  d.amount = 100000;
  d.rate = 9.5;
  d.pay_period = 30;
  const int terms[]{1, 3, 6, 7, 8, 9, 12, 36, 60};
  double expected_results[]{806.85,  2394.52, 4789.04,  5569.86, 6375.78,
                            7180.42, 9516.48, 28499.97, 47516.45};
  for (int i = 0; i < 7; i++) {
    DBPRINT("%d, term=%d\n", i, terms[i]);
    d.duration = terms[i];
    calc.setData(&d);
    calc.calculate();
    d = *calc.getData();
    ASSERT_NEAR(d.interest, expected_results[i], kTestFinanceEpsilon);
  }
}

TEST(DepositTest, cap) {
  DepositModel calc;
  DepositModelData d;
  d.amount = 100000;
  d.rate = 9.5;
  d.pay_period = 30;
  d.int_cap = true;
  int const terms[]{1, 3, 6, 7, 8, 9, 12, 36, 60};
  double expected_results[]{806.85,  2413.68, 4885.62,  5704.59, 6556.49,
                            7413.89, 9942.74, 32826.95, 60527.02};
  for (int i = 0; i < 7; i++) {
    DBPRINT("%d, term=%d\n", i, terms[i]);
    d.duration = terms[i];
    calc.setData(&d);
    calc.calculate();
    d = *calc.getData();
    ASSERT_NEAR(d.interest, expected_results[i], kTestFinanceEpsilon);
  }
}

TEST(DepositTest, pay_periods_no_cap) {
  DepositModel calc;
  DepositModelData d;
  d.amount = 100000;
  d.duration = 60;
  d.rate = 9.5;
  int const periods[]{1, 7, 30, 90, 180, 360};
  double expected_results[]{47521.81, 47515.89, 47516.45, 47516.51,
                            47516.51, 47516.50, 47516.50};
  for (int i = 0; i < 6; i++) {
    DBPRINT("%d, term=%d\n", i, periods[i]);
    d.pay_period = periods[i];
    calc.setData(&d);
    calc.calculate();
    d = *calc.getData();
#ifndef NDEBUG
    if (i == 3)
      for (auto a : d.interests) std::cout << a << std::endl;
#endif
    ASSERT_NEAR(d.interest, expected_results[i], kTestFinanceEpsilon);
  }
}

TEST(DepositTest, pay_periods_cap) {
  DepositModel calc;
  DepositModelData d;
  d.amount = 100000;
  d.duration = 60;
  d.rate = 9.5;
  d.int_cap = true;
  const int periods[]{1, 7, 30, 90, 180, 360};
  double expected_results[]{60818.37, 60758.53, 60527.02,
                            59936.63, 59077.40, 57447.6};
  for (int i = 0; i < 6; i++) {
    DBPRINT("%d, term=%d\n", i, periods[i]);
    d.pay_period = periods[i];
    calc.setData(&d);
    calc.calculate();
    d = *calc.getData();
    ASSERT_NEAR(d.interest, expected_results[i], kTestFinanceEpsilon);
  }
}

TEST(DepositTest, longer_period) {
  DepositModel calc;
  DepositModelData d;
  d.amount = 100000;
  d.duration = 3;
  d.rate = 9.5;
  d.int_cap = true;
  int const periods[]{1, 7, 30, 90, 180, 360};
  double expected_results[]{2423.10, 2421.21, 2413.68,
                            2394.52, 2394.52, 2394.52};
  for (int i = 0; i < 6; i++) {
    DBPRINT("%d, term=%d\n", i, periods[i]);
    d.pay_period = periods[i];
    calc.setData(&d);
    calc.calculate();
    d = *calc.getData();
    ASSERT_NEAR(d.interest, expected_results[i], kTestFinanceEpsilon);
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}