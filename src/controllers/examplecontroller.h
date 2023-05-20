#ifndef _SRC_CONTROLLERS_EXAMPLECONTROLLER_H
#define _SRC_CONTROLLERS_EXAMPLECONTROLLER_H

#include "../models/basemodel.h"
namespace s21 {
using BaseModel = AbstractModel<BaseCalcData>;

class ExampleController {
 public:
  explicit ExampleController(BaseModel *m) : model_(m){};
  void userAction(const BaseCalcData *d) {
    model_->setData(d);
    model_->calculate();
  };

 private:
  BaseModel *model_;
};
}  // namespace s21
#endif  //_SRC_CONTROLLERS_EXAMPLECONTROLLER_H