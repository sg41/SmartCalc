#include "controllers/exampleController.h"
#include "views/consoleView.h"

using namespace s21;

int main() {
  GraphModel model;
  ExampleController controller((BaseModel*)&model);
  ConsoleView view(&controller);
  model.registerObserver(&view);
  view.startEventLoop();
  return 0;
}