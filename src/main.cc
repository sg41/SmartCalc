#include "controllers/exampleController.h"
#include "views/consoleView.h"

int main() {
  GraphModel model;
  ExampleController controller(&model);
  ConsoleView view(&controller);
  model.register_observer(&view);
  view.startEventLoop();
  return 0;
}