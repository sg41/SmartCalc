#include "consoleView.h"

using std::cin;
using std::cout;
using std::endl;
using namespace s21;

void ConsoleView::displayMenu() {
  std::cout << "=========" << std::endl;
  std::cout << " M E N U " << std::endl;
  std::cout << "=========" << std::endl;
  std::cout << "1. Enter x value (Current: " << this->data.x << ")"
            << std::endl;
  std::cout << "2. Enter formula (Current: \"" << this->data.str << "\")"
            << std::endl;
  std::cout << "3. Calculate formula" << std::endl;
  std::cout << "4. Change x range (Current: from " << this->data.MINX << ", to "
            << data.MAXX << ")" << std::endl;
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
  getline(cin, data.str);  // vipe previous eol
  getline(cin, data.str);
  return data.str;
}

void ConsoleView::draw_txt_graph() {
  int i, j;
  for (j = 0; j < data.MAXJ; j++) {
    for (i = 0; i < data.MAXI; i++) {
      if ((int)round(-data.y_vect[i] * data.dy + round(data.MAXJ / 2)) == j) {
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
      case VAR:
        data.x = performNumericInput();
        break;

      case FORMULA:
        performFormulaInput();
        break;

      case CALC:
        try {
          controller->userAction(&data);
        } catch (const std::invalid_argument &e) {
          cout << "ERROR: " << e.what() << std::endl;
        };
        break;

      case RANGE:
        cout << "MIN X: ";
        data.MINX = performNumericInput();
        cout << "MAX X: ";
        data.MAXX = performNumericInput();
        set_scale();
        break;
      case EXIT:
        done = true;
        break;

      case NONE:
      default:
        std::cout << "Wrong menu item number!";
        continue;
    }
  }
}