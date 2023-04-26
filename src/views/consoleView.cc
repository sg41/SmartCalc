#include "consoleView.h"

#include <iostream>

using std::cin;
using std::cout;
using std::endl;

void ConsoleView::displayMenu() {
  std::cout << "=========" << std::endl;
  std::cout << " M E N U " << std::endl;
  std::cout << "=========" << std::endl;
  std::cout << "1. Enter x value" << std::endl;
  std::cout << "2. Enter formula" << std::endl;
  std::cout << "3. Calculate formula" << std::endl;
  std::cout << "4. Show formula graph" << std::endl;
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
  std::string formula;
  std::cout << "Input a decimal number: ";
  std::cin >> formula;
  return formula;
}

void ConsoleView::startEventLoop() {
  while (true) {
    double result;
    std::string form;
    displayMenu();
    switch ((Choice)performChoice()) {
      case VAR:
        result = performNumericInput();
        break;

      case FORMULA:
        form = performFormulaInput();
        break;

      case 4:
        controller->calcalate();
        // std::cout << "Current result is: " << controller->calculate(result)
        //           << std::endl;
        break;

      case DIV:
        result = controller->div(performNumericInput());
        break;

      case RES:
        controller->reset();
        result = 0.0;
        break;

      case EXIT:
        exit(1);

      default:
        std::cout << "Wrong menu item number!";
        continue;
    }
  }
}