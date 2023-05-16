#ifndef _SRC_MODELS_DEPOSITMODEL_H_
#define _SRC_MODELS_DEPOSITMODEL_H_

#include <chrono>
#include <vector>

#include "../rpn_cpp/core.h"
#include "creditModel.h"

namespace s21 {
const static int kSecondsPerDay = 86400;

struct DepositModelData : public CreditModelData {
 public:
  using CreditModelData::CreditModelData;
  DepositModelData() { DepositModelData::initData(); };

  double tax_rate;
  double tax;
  bool int_cap;
  double interest;
  std::vector<double> interests;
  std::vector<double> replenishments;
  std::vector<double> withdrawals;
  double replenishment;
  double withdrawal;

  void initData() override {
    CreditModelData::initData();
    tax_rate = 0;
    tax = 0;
    int_cap = false;
    interest = 0;
    withdrawals.clear();
    replenishments.clear();
    interests.clear();
    withdrawal = 0;
    replenishment = 0;
  };
};

class DepositModel : public AbstractModel<DepositModelData> {
 public:
  using AbstractModel<DepositModelData>::AbstractModel;

  void calculate() override {
    data_->total_payment = calcComplexInterest();
    data_->tax = (data_->interest) * (data_->tax_rate / 100.);
    data_->total_payment -= data_->tax;
    notifyObservers();
  };

 protected:
  long double calcSimpleDailyInterest(long double sum, int startday, int days);
  double calcComplexInterest();
};
}  // namespace s21
#endif  //_SRC_MODELS_DEPOSITMODEL_H_