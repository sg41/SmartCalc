#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
  QApplication::setGraphicsSystem("raster");
#endif
  QApplication a(argc, argv);

  //  QTranslator translator;
  //  const QStringList uiLanguages = QLocale::system().uiLanguages();
  //  for (const QString &locale : uiLanguages) {
  //    const QString baseName = "Calc_2_0_" + QLocale(locale).name();
  //    if (translator.load(":/i18n/" + baseName)) {
  //      a.installTranslator(&translator);
  //      break;
  //    }
  //  }
  setlocale(LC_ALL, "en_US.UTF-8");
  MainWindow w;
  w.show();
  return a.exec();
}
