#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <iostream>

#include "../consoleview/consoleview.h"
#include "../controllers/examplecontroller.h"
#include "mainwindow.h"

int main(int argc, char *argv[]) {
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
  QApplication::setGraphicsSystem("raster");
#endif
  int res = 0;
#ifdef Q_OS_LINUX
  bool useGUI = getenv("DISPLAY") != 0;
#else
  bool useGUI = true;
#endif
  if (useGUI) {
    QApplication a(argc, argv);
    QLocale l = QLocale::c();
    l.setNumberOptions(QLocale::DefaultNumberOptions);
    QLocale::setDefault(l);
    MainWindow w;
    // Setup Icon
    QIcon ic(":/icon/calc_icon_64.png");
    w.setWindowIcon(ic);
    w.show();
    res = a.exec();
  } else {
    using namespace s21;
    GraphModel model;
    ExampleController controller((BaseModel *)&model);
    ConsoleView view(&controller);
    model.registerObserver(&view);
    view.startEventLoop();
  }
  return res;
}
