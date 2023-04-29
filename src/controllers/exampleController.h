#ifndef EXAMPLECONTROLLER_H
#define EXAMPLECONTROLLER_H

#include "../models/graphModel.h"

class ExampleController {
 private:
  GraphModel *model;

 public:
  explicit ExampleController(GraphModel *m) : model(m){};
  void user_action(const GraphModelData *d) {
    model->set_data(d);
    model->calculate();
  };
};

#endif