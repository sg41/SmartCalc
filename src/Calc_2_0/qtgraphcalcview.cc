#include "qtgraphcalcview.h"

#include "ui_qtgraphcalcview.h"

QtGraphCalcView::QtGraphCalcView(QWidget *parent)
    : QWidget(parent), controller(&model), ui(new Ui::QtGraphCalcView) {
  ui->setupUi(this);

  model.register_observer(this);

  showSizeDialog(false);
  ui->graph_area->xAxis->setRange(this->m_data.MINX, this->m_data.MAXX);
  ui->graph_area->yAxis->setRange(this->m_data.MINY, this->m_data.MAXY);

  // keyboard mapping
  QWidget *keyboard = findChild<QWidget *>(QString("Keyboard"));
  QList<QPushButton *> buttons = keyboard->findChildren<QPushButton *>();
  QSignalMapper *signalMapper = new QSignalMapper(this);
  for (auto b : buttons) {
    b->setFocusPolicy(Qt::NoFocus);
    signalMapper->setMapping(b, b->text());
    connect(b, SIGNAL(clicked()), signalMapper, SLOT(map()));
  }
  connect(signalMapper, SIGNAL(mapped(const QString &)), this,
          SLOT(buttonPressed(const QString &)));
  // X value validator
  QDoubleValidator *xValidator = new QDoubleValidator(this);
  ui->X_Value->setValidator(xValidator);
}

QtGraphCalcView::~QtGraphCalcView() { delete ui; }

void QtGraphCalcView::on_graph_area_customContextMenuRequested(
    const QPoint &pos) {
  ui->SizeDialog->findChild<QSpinBox *>(QString("spinBox_MAXX"))
      ->setValue(this->m_data.MAXX);
  ui->SizeDialog->findChild<QSpinBox *>(QString("spinBox_MINX"))
      ->setValue(this->m_data.MINX);
  ui->SizeDialog->findChild<QSpinBox *>(QString("spinBox_MAXY"))
      ->setValue(this->m_data.MAXY);
  ui->SizeDialog->findChild<QSpinBox *>(QString("spinBox_MINY"))
      ->setValue(this->m_data.MINY);
  //  ui->SizeDialog->move(pos);
  showSizeDialog(true);
}

void QtGraphCalcView::on_buttonBox_accepted() {
  //! value to be checked!
  this->m_data.MAXX =
      ui->SizeDialog->findChild<QSpinBox *>(QString("spinBox_MAXX"))->value();
  this->m_data.MINX =
      ui->SizeDialog->findChild<QSpinBox *>(QString("spinBox_MINX"))->value();
  this->m_data.MAXY =
      ui->SizeDialog->findChild<QSpinBox *>(QString("spinBox_MAXY"))->value();
  this->m_data.MINY =
      ui->SizeDialog->findChild<QSpinBox *>(QString("spinBox_MINY"))->value();
  ui->graph_area->xAxis->setRange(this->m_data.MINX, this->m_data.MAXX);
  ui->graph_area->yAxis->setRange(this->m_data.MINY, this->m_data.MAXY);
  showSizeDialog(false);
  ui->graph_area->replot();
}

void QtGraphCalcView::on_buttonBox_rejected() { showSizeDialog(false); }

void QtGraphCalcView::buttonPressed(const QString &str) {
  if (str == QString("AC")) {
    ui->InputStr->clear();
  } else if (str == QString("Bksp")) {
    ui->InputStr->backspace();
  } else if (str.length() > 1 && str != QString("mod")) {
    ui->InputStr->insert(str + QString("("));
  } else if (str == QString("=")) {
    m_data.str = ui->InputStr->text().toStdString();
    emit result_requested();
  } else {
    ui->InputStr->insert(str);
  }
}

void QtGraphCalcView::showSizeDialog(bool on) {
  if (ui->SizeDialog) {
    ui->SizeDialog->setVisible(on);
  }
}

void QtGraphCalcView::observer_update(const GraphModelData *model_data) {
  m_data = *(model_data);
  ui->graph_area->replot();
  ui->listWidget->insertItem(
      0, ui->InputStr->text() + " = " + std::to_string(m_data.y).c_str());
  ui->listWidget->item(0)->setTextAlignment(Qt::AlignRight);
  ui->InputStr->setText(QString(std::to_string(m_data.y).c_str()));
  emit showStatus(std::to_string(m_data.y));
};

void QtGraphCalcView::on_QtGraphCalcView_result_requested() {
  try {
    controller.user_action(&m_data);
  } catch (std::invalid_argument &e) {
    emit showStatus(e.what());
  }
}

void QtGraphCalcView::onHistoryItemDblClicked(QListWidgetItem *history) {
  std::string str = history->text().toStdString();
  ui->InputStr->setText(QString(str.substr(0, str.find(" = ")).c_str()));
}

void QtGraphCalcView::on_X_Value_textChanged(const QString &arg1) {
  m_data.x = arg1.toDouble();
  emit showStatus(std::string("X = ") + std::to_string(m_data.x));
}
