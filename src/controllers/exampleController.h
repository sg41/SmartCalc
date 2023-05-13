#ifndef EXAMPLECONTROLLER_H
#define EXAMPLECONTROLLER_H

#include "../models/baseModel.h"
namespace s21 {
using BaseModel = AbstractModel<BaseCalcData>;

class ExampleController {
 private:
  BaseModel *model;

 public:
  explicit ExampleController(BaseModel *m) : model(m){};
  void user_action(const BaseCalcData *d) {
    model->set_data(d);
    model->calculate();
  };
};
}  // namespace s21
#endif