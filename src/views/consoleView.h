#ifndef CONSOLEVIEW_H
#define CONSOLEVIEW_H

#include "../models/graphModel.h"
#include "exampleController.h"

enum Choice { VAR = 1, FORMULA = 2, CALC = 3, GRAPH = 4, EXIT = 0, NONE = -1 };

class ConsoleView : public CalcObserverInterface<GraphModelData> {
 private:
  ExampleController *controller;

 public:
  ConsoleView(ExampleController *c) : controller(c){};
  void displayMenu();
  int performChoice();
  double performNumericInput();
  std::string performFormulaInput();
  void startEventLoop();
  void update(ModelObservableInterface<GraphModelData> *m) override {
    std::cout << "Current result is: " << m->get_data().y << std::endl;
  };
};

#endif