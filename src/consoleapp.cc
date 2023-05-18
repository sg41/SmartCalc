#include "consoleview/consoleview.h"
#include "controllers/examplecontroller.h"

using namespace s21;

int main() {
  GraphModel model;
  ExampleController controller((BaseModel*)&model);
  ConsoleView view(&controller);
  model.registerObserver(&view);
  view.startEventLoop();
  return 0;
}