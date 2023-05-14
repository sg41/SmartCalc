#ifndef _DEPOSITMODEL_H_
#define _DEPOSITMODEL_H_

#include <chrono>
#include <vector>

#include "../rpn_cpp/core.h"
#include "creditModel.h"

namespace s21 {
const static int SECOND_PER_DAY = 86400;

class DepositModelData : public CreditModelData {
 public:
  using CreditModelData::CreditModelData;
  DepositModelData() { DepositModelData::init_data(); };

 public:
  double tax_rate;
  double tax;
  bool int_cap;
  double interest;
  std::vector<double> interests;
  std::vector<double> replenishments;
  std::vector<double> withdrawals;
  double replenishment;
  double withdrawal;

  void init_data() override {
    CreditModelData::init_data();
    tax_rate = 0;
    tax = 0;
    int_cap = false;
    interest = 0;
    withdrawals.clear();
    replenishments.clear();
    withdrawal = 0;
    replenishment = 0;
  };
};

class DepositModel : public AbstractModel<DepositModelData> {
 public:
  using AbstractModel<DepositModelData>::AbstractModel;

  void calculate() override {
    data->total_payment = complex_interest_calc();
    data->tax = (data->interest) * (data->tax_rate / 100.);
    data->total_payment -= data->tax;
    notify_observers();
  };

 protected:
  long double calc_simple_daily_interest(long double sum, int startday,
                                         int days);
  double complex_interest_calc();
};
}  // namespace s21
#endif  //_DEPOSITMODEL_H_