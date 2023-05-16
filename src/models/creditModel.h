#ifndef _SRC_MODELS_CREDITMODEL_H_
#define _SRC_MODELS_CREDITMODEL_H_

#include <vector>

#include "../rpn_cpp/core.h"
#include "baseModel.h"

namespace s21 {
struct CreditModelData : public BaseCalcData {
 public:
  using BaseCalcData::BaseCalcData;
  CreditModelData() { CreditModelData::initData(); };

 public:
  std::string str;
  double amount;
  int duration;
  int pay_period;
  double rate;
  double monthly_payment;
  double monthly_payment_min;
  double overpayment;
  double total_payment;
  int type;
  bool round;
  std::vector<double> monthly_payments;
  static const int kAnnuity = 0;
  static const int kDifferentiated = 1;

  void initData() override {
    BaseCalcData::initData();
    amount = 0;
    duration = 0;
    pay_period = 0;
    rate = 0;
    monthly_payment = 0;
    monthly_payment_min = 0;
    overpayment = 0;
    total_payment = 0;
    type = 0;
    round = false;
    monthly_payments.clear();
  };
};

class CreditModel : public AbstractModel<CreditModelData> {
 public:
  using AbstractModel<CreditModelData>::AbstractModel;

  void calculate() override {
    if (data_->type == data_->kAnnuity) {
      std::string monthly_payment_expr =
          std::to_string(data_->amount) + "*(x/100/12*(x/100/12+1)^" +
          std::to_string(data_->duration) + "/((1+x/100/12)^" +
          std::to_string(data_->duration) + "-1))";
      calculator_.makeRpnExpr(monthly_payment_expr);
      data_->monthly_payment =
          round(calculator_.calc(data_->rate) * 100.) / 100.;
      //! maybe to change to throw
      if (data_->round && data_->monthly_payment < 1) {
        data_->error = 2;
        data_->error_message = "Incorrect input data - can't calculate";
      }
      if (data_->round) data_->monthly_payment = round(data_->monthly_payment);
      data_->total_payment = data_->monthly_payment * data_->duration;
    } else {  // DIFFERENTIATED
      std::string monthly_payment_expr =
          std::to_string(data_->amount) + "/" +
          std::to_string(data_->duration) + "+(" +
          std::to_string(data_->amount) + "-(" + std::to_string(data_->amount) +
          "/" + std::to_string(data_->duration) + ")*x)*(" +
          std::to_string(data_->rate) + "/100/12)";

      calculator_.makeRpnExpr(monthly_payment_expr);
      data_->monthly_payments.clear();
      data_->total_payment = 0;
      for (int m = 0; m < data_->duration; m++) {
        data_->monthly_payments.push_back(
            round(calculator_.calc((double)m) * 100.) / 100.);
        if (data_->round) {
          data_->monthly_payments[m] = round(data_->monthly_payments[m]);
        }
        data_->total_payment += data_->monthly_payments[m];
      }
      data_->monthly_payment = data_->monthly_payments[0];
      data_->monthly_payment_min = data_->monthly_payments[data_->duration - 1];
    }
    data_->overpayment = data_->total_payment - data_->amount;
    notifyObservers();
  };

 protected:
  CalcCore calculator_;
};
}  // namespace s21
#endif  //_SRC_MODELS_CREDITMODEL_H_