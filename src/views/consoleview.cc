#include "consoleview.h"

using std::cin;
using std::cout;
using std::endl;
using namespace s21;

void ConsoleView::displayMenu() {
  std::cout << "=========" << std::endl;
  std::cout << " M E N U " << std::endl;
  std::cout << "=========" << std::endl;
  std::cout << "1. Enter x value (Current: " << this->data_.x << ")"
            << std::endl;
  std::cout << "2. Enter formula (Current: \"" << this->data_.str << "\")"
            << std::endl;
  std::cout << "3. Calculate formula" << std::endl;
  std::cout << "4. Change x range (Current: from " << this->data_.min_x
            << ", to " << data_.max_x << ")" << std::endl;
  std::cout << "0. Quit" << std::endl << std::endl;
}

int ConsoleView::performChoice() {
  int choice;
  std::cout << "Input a menu item digit: ";
  std::cin >> choice;
  return choice;
}

double ConsoleView::performNumericInput() {
  double number;
  std::cout << "Input a decimal number: ";
  std::cin >> number;
  return number;
}

std::string ConsoleView::performFormulaInput() {
  std::cout << "Input a formula string: ";
  getline(cin, data_.str);  // vipe previous eol
  getline(cin, data_.str);
  return data_.str;
}

void ConsoleView::drawTxtGraph() {
  int i, j;
  for (j = 0; j < data_.max_console_height; j++) {
    for (i = 0; i < data_.max_console_width; i++) {
      if ((int)round(-data_.y_vect[i] * data_.dy +
                     round(data_.max_console_height / 2)) == j) {
        std::cout << ("*");
      } else {
        std::cout << (".");
      }
    }
    std::cout << ("\n");
  }
};

void ConsoleView::startEventLoop() {
  bool done = false;
  while (!done) {
    displayMenu();
    switch ((Choice)performChoice()) {
      case kVar:
        data_.x = performNumericInput();
        break;

      case kFormula:
        performFormulaInput();
        break;

      case kCalc:
        try {
          controller->userAction(&data_);
        } catch (const std::invalid_argument &e) {
          cout << "ERROR: " << e.what() << std::endl;
        };
        break;

      case kRange:
        cout << "MIN X: ";
        data_.min_x = performNumericInput();
        cout << "MAX X: ";
        data_.max_x = performNumericInput();
        setScale();
        break;
      case kExit:
        done = true;
        break;

      case kNone:
      default:
        std::cout << "Wrong menu item number!";
        continue;
    }
  }
}