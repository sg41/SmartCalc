#ifndef EXAMPLECONTROLLER_H
#define EXAMPLECONTROLLER_H

// #include "../models/graphModel.h"
// namespace s21 {
// class ExampleController {
//  private:
//   GraphModel *model;

//  public:
//   explicit ExampleController(GraphModel *m) : model(m){};
//   void user_action(const GraphModelData *d) {
//     model->set_data(d);
//     model->calculate();
//   };
// };
// }  // namespace s21
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