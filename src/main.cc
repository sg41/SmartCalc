#include "controllers/exampleController.h"
#include "views/consoleView.h"

int main() {
  GraphModel model;
  ExampleController controller(&model);
  ConsoleView view(&controller);
  // controller.register_view(&view);
  model.register_observer(&view);
  view.startEventLoop();
  return 1;
}