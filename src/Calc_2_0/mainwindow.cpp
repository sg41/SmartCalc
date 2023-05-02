#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), controller(&model), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  // setup child elements
  SizeDialog = ui->centralwidget->findChild<QWidget *>(QString("SizeDialog"));
  GraphCalcView =
      ui->centralwidget->findChild<QtGraphCalcView *>(QString("GraphCalcView"));
  graph_area = GraphCalcView->findChild<QCustomPlot *>(QString("graph_area"));
  InputStr = GraphCalcView->findChild<QLineEdit *>(QString("InputStr"));
  if (!SizeDialog || !GraphCalcView || !graph_area)
    throw std::invalid_argument("Interface elements missing");

  showSizeDialog(false);
  model.register_observer(GraphCalcView);
  graph_area->xAxis->setRange(GraphCalcView->m_data.MINX,
                              GraphCalcView->m_data.MAXX);
  graph_area->yAxis->setRange(GraphCalcView->m_data.MINY,
                              GraphCalcView->m_data.MAXY);
  // keyboard mapping
  QWidget *keyboard =
      ui->centralwidget->findChild<QWidget *>(QString("Keyboard"));
  QList<QPushButton *> buttons = keyboard->findChildren<QPushButton *>();
  signalMapper = new QSignalMapper(this);
  for (auto b : buttons) {
    b->setFocusPolicy(Qt::NoFocus);
    signalMapper->setMapping(b, b->text());
    connect(b, SIGNAL(clicked()), signalMapper, SLOT(map()));
  }
  connect(signalMapper, SIGNAL(mapped(const QString &)), this,
          SLOT(buttonPressed(const QString &)));
}

MainWindow::~MainWindow() {
  delete ui;
  delete signalMapper;
}

void MainWindow::on_graph_area_customContextMenuRequested(const QPoint &pos) {
  SizeDialog->findChild<QSpinBox *>(QString("spinBox_MAXX"))
      ->setValue(GraphCalcView->m_data.MAXX);
  SizeDialog->findChild<QSpinBox *>(QString("spinBox_MINX"))
      ->setValue(GraphCalcView->m_data.MINX);
  SizeDialog->findChild<QSpinBox *>(QString("spinBox_MAXY"))
      ->setValue(GraphCalcView->m_data.MAXY);
  SizeDialog->findChild<QSpinBox *>(QString("spinBox_MINY"))
      ->setValue(GraphCalcView->m_data.MINY);
  //  SizeDialog->move(pos);
  showSizeDialog(true);
}

void MainWindow::on_buttonBox_accepted() {
  GraphCalcView->m_data.MAXX =
      SizeDialog->findChild<QSpinBox *>(QString("spinBox_MAXX"))->value();
  GraphCalcView->m_data.MINX =
      SizeDialog->findChild<QSpinBox *>(QString("spinBox_MINX"))->value();
  GraphCalcView->m_data.MAXY =
      SizeDialog->findChild<QSpinBox *>(QString("spinBox_MAXY"))->value();
  GraphCalcView->m_data.MINY =
      SizeDialog->findChild<QSpinBox *>(QString("spinBox_MINY"))->value();
  graph_area->xAxis->setRange(GraphCalcView->m_data.MINX,
                              GraphCalcView->m_data.MAXX);
  graph_area->yAxis->setRange(GraphCalcView->m_data.MINY,
                              GraphCalcView->m_data.MAXY);
  showSizeDialog(false);
  graph_area->replot();
}

void MainWindow::on_buttonBox_rejected() { showSizeDialog(false); }

void MainWindow::buttonPressed(const QString &str) {
  if (str == QString("AC")) {
    InputStr->clear();
  } else if (str == QString("Bksp")) {
    InputStr->backspace();
  } else if (str.length() > 1 && str != QString("mod")) {
    InputStr->insert(str + QString("("));
  } else {
    InputStr->insert(str);
  }
}

void MainWindow::showSizeDialog(bool on) {
  if (SizeDialog) {
    SizeDialog->setVisible(on);
  }
}
