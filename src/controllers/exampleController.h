#ifndef EXAMPLECONTROLLER_H
#define EXAMPLECONTROLLER_H

#include "../models/baseModel.h"
namespace s21 {
using BaseModel = AbstractModel<BaseCalcData>;

class ExampleController {
 private:
  BaseModel *model_;

 public:
  explicit ExampleController(BaseModel *m) : model_(m){};
  void userAction(const BaseCalcData *d) {
    model_->set_data(d);
    model_->calculate();
  };
};
}  // namespace s21
#endif