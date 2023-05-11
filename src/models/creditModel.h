#ifndef _CREDITMODEL_H_
#define _CREDITMODEL_H_

#include <vector>

#include "../rpn_cpp/core.h"
#include "baseModel.h"

namespace s21 {
class CreditModelData : public BaseCalcData {
 public:
  using BaseCalcData::BaseCalcData;
  CreditModelData() { CreditModelData::init_data(); };
  int validate_data() override {
    int err = 0;
    // if (MINX >= VERY_MIN_X && MINY >= VERY_MIN_Y && MAXX <= VERY_MAX_X &&
    //     MAXY <= VERY_MAX_Y)
    //   err = 0;
    // else
    //   err = 1;
    return err;
  };

 public:
  std::string str;
  double amount;
  int duration;
  int pay_period;
  double rate;
  double tax_rate;
  double monthly_payment;
  double monthly_payment_min;
  double overpayment;
  double total_payment;
  int type;
  bool round;
  int int_cap;
  std::vector<double> monthly_payments;

  void init_data() override {
    BaseCalcData::init_data();
    amount = 0;
    duration = 0;
    pay_period = 0;
    rate = 0;
    tax_rate = 0;
    monthly_payment = 0;
    monthly_payment_min = 0;
    overpayment = 0;
    total_payment = 0;
    type = 0;
    round = false;
    int_cap = 0;
    monthly_payments.clear();
  };
};

class CreditModel : public AbstractModel<CreditModelData> {
 public:
  using AbstractModel<CreditModelData>::AbstractModel;

  void calculate() override {
    if (data->type == data->ANNUITET) {
      std::string monthly_payment_expr =
          std::to_string(data->amount) + "*(x/100/12*(x/100/12+1)^" +
          std::to_string(data->duration) + "/((1+x/100/12)^" +
          std::to_string(data->duration) + "-1))";
      c.make_rpn_expr(monthly_payment_expr);
      data->monthly_payment = round(c.calc(data->rate) * 100.) / 100.;
      //! maybe to change to throw
      if (data->round && data->monthly_payment < 1) {
        data->error = 2;
        data->error_message = "Incorrect input data - can't calculate";
      }
      if (data->round) data->monthly_payment = round(data->monthly_payment);
      data->total_payment = data->monthly_payment * data->duration;
    } else {  // DIFFERENTIATED
      std::string monthly_payment_expr =
          std::to_string(data->amount) + "/" + std::to_string(data->duration) +
          "+(" + std::to_string(data->amount) + "-(" +
          std::to_string(data->amount) + "/" + std::to_string(data->duration) +
          ")*x)*(" + std::to_string(data->rate) + "/100/12)";

      c.make_rpn_expr(monthly_payment_expr);
      data->monthly_payments.clear();
      data->total_payment = 0;
      for (int m = 0; m < data->duration; m++) {
        data->monthly_payments.push_back(round(c.calc((double)m) * 100.) /
                                         100.);
        if (data->round)
          data->monthly_payments[m] = round(data->monthly_payments[m]);
        data->total_payment += data->monthly_payments[m];
      }
      data->monthly_payment = data->monthly_payments[0];
      data->monthly_payment_min = data->monthly_payments[data->duration - 1];
    }
    data->overpayment = data->total_payment - data->amount;
    notify_observers();
  };

 protected:
  CalcCore c;
};
}  // namespace s21
#endif  //_CREDITMODEL_H_