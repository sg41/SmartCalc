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
 public:
  explicit ConsoleView(ExampleController *c) : controller(c) {
    data_.clip_x1 = 0;
    data_.clip_x2 = data_.max_console_width;
    data_.clip_y1 = 0;
    data_.clip_y2 = data_.max_console_height;
  };
  void startEventLoop();
  void observerUpdate(const GraphModelData *model_data) override {
    data_ = *(model_data);
    std::cout << "Current result for X=" << data_.x << " is: " << data_.y
              << std::endl;
    drawTxtGraph();
  };

 private:
  ExampleController *controller = nullptr;
  GraphModelData data_;

  void drawTxtGraph();
  void displayMenu();
  int performChoice();
  double performNumericInput();
  std::string performFormulaInput();
};
}  // namespace s21
#endif