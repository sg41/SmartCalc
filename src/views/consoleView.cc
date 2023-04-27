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
  std::cout << "0. Quit" << std::endl << std::endl;
}

int ConsoleView::performChoice() {
  int choice;
  std::cout << "Input a menu item digit: ";
  std::cin >> choice;
  return choice;
}

double ConsoleView::performNumericInput() {
  // double number;
  std::cout << "Input a decimal number: ";
  std::cin >> data->x;
  return data->x;
}

std::string ConsoleView::performFormulaInput() {
  // std::string formula;
  std::cout << "Input a decimal number: ";
  std::cin >> data->str;
  return data->str;
}

void ConsoleView::draw_txt_graph() {
  // double scalex = (double)(data->MAXX - data->MINX) / data->MAXI;
  // double scaley = (double)data->MAXJ / (data->MAXY - data->MINY);
  int i, j;
  for (j = 0; j < data->MAXJ; j++) {
    // double x = datat->MINX;
    for (i = 0; i < data->MAXI; i++) {
      if ((int)round(data->y_vect[i] * data->dy + round(data->MAXJ / 2)) == j) {
        printf("*");
      } else {
        printf(".");
      }
      // x += scalex;
    }
    printf("\n");
  }
};

void ConsoleView::startEventLoop() {
  while (true) {
    displayMenu();
    switch ((Choice)performChoice()) {
      case VAR:
        performNumericInput();
        break;

      case FORMULA:
        performFormulaInput();
        break;

      case CALC:
        controller->calculate();
        break;

      case EXIT:
        exit(1);

      default:
        std::cout << "Wrong menu item number!";
        continue;
    }
  }
}