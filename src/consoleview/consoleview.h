#ifndef CONSOLEVIEW_H
#define CONSOLEVIEW_H
#include <iostream>

#include "../controllers/examplecontroller.h"
#include "../models/graphmodel.h"
namespace s21 {
enum Choice {
  kVar = 1,
  kFormula = 2,
  kCalc = 3,
  kRange = 4,
  kExit = 0,
  kNone = -1
};

class ConsoleView : public ModelObserverInterface<GraphModelData> {
 private:
  ExampleController *controller = nullptr;
  GraphModelData data_;
  void setScale() {
    data_.dx = (double)(data_.max_x - data_.min_x) / data_.max_console_width;
    data_.dy = (double)data_.max_console_height / (data_.max_y - data_.min_y);
  };
  void drawTxtGraph();
  void displayMenu();
  int performChoice();
  double performNumericInput();
  std::string performFormulaInput();

 public:
  explicit ConsoleView(ExampleController *c) : controller(c) { setScale(); };
  void startEventLoop();
  void observerUpdate(const GraphModelData *model_data) override {
    data_ = *(model_data);
    std::cout << "Current result for X=" << data_.x << " is: " << data_.y
              << std::endl;
    drawTxtGraph();
  };
};
}  // namespace s21
#endif