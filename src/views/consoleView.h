#ifndef CONSOLEVIEW_H
#define CONSOLEVIEW_H

#include "../models/graphModel.h"
#include "exampleController.h"

enum Choice { VAR = 1, FORMULA = 2, CALC = 3, EXIT = 0, NONE = -1 };

class ConsoleView : public CalcObserverInterface<GraphModelData> {
 private:
  ExampleController *controller;
  GraphModelData *data;

 public:
  ConsoleView(ExampleController *c) : controller(c), data() {
    data->dx = (double)(data->MAXX - data->MINX) / data->MAXI;
    data->dy = (double)data->MAXJ / (data->MAXY - data->MINY);
  };
  ~ConsoleView() { delete data; };
  void displayMenu();
  int performChoice();
  double performNumericInput();
  std::string performFormulaInput();
  void startEventLoop();
  void draw_txt_graph();
  void update(ModelObservableInterface<GraphModelData> *model) override {
    *data = *(model->get_data());
    std::cout << "Current result for X=" << data->x << " is: " << data->y
              << std::endl;
    draw_txt_graph();
  };
};

#endif