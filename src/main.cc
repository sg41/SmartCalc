#include "controllers/examplecontroller.h"
#include "views/consoleview.h"

using namespace s21;

int main() {
  GraphModel model;
  ExampleController controller((BaseModel*)&model);
  ConsoleView view(&controller);
  model.registerObserver(&view);
  view.startEventLoop();
  return 0;
}