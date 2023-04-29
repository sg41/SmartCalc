#ifndef CONSOLEVIEW_H
#define CONSOLEVIEW_H
#include <iostream>

#include "../controllers/exampleController.h"
#include "../models/graphModel.h"

enum Choice { VAR = 1, FORMULA = 2, CALC = 3, RANGE = 4, EXIT = 0, NONE = -1 };

class ConsoleView : public ModelObserverInterface<GraphModelData> {
 private:
  ExampleController *controller = nullptr;
  GraphModelData *data = nullptr;
  // GraphModelData g_data;
  void set_scale() {
    data->dx = (double)(data->MAXX - data->MINX) / data->MAXI;
    data->dy = (double)data->MAXJ / (data->MAXY - data->MINY);
  };
  void draw_txt_graph();
  void displayMenu();
  int performChoice();
  double performNumericInput();
  std::string performFormulaInput();

 public:
  explicit ConsoleView(ExampleController *c) : controller(c) {
    data = new GraphModelData;
    set_scale();
  };
  // ConsoleView(const ConsoleView &c) {
  //   data = new GraphModelData(*c.data);
  //   controller = c.controller;
  // };
  // ConsoleView(ConsoleView &&c) {
  //   data = c.data;
  //   controller = c.controller;
  //   c.data = nullptr;
  // };
  ~ConsoleView() { delete data; };
  void startEventLoop();
  void update(const GraphModelData *model_data) override {
    *data = *(model_data);
    std::cout << "Current result for X=" << data->x << " is: " << data->y
              << std::endl;
    draw_txt_graph();
  };
};

#endif