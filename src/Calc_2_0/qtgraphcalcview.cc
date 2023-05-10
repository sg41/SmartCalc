#include "qtgraphcalcview.h"

#include "ui_qtgraphcalcview.h"

QtGraphCalcView::QtGraphCalcView(QWidget *parent)
    : QWidget(parent),
      controller((BaseModel *)&model),
      ui(new Ui::QtGraphCalcView) {
  // Setup interface
  ui->setupUi(this);
  showSizeDialog(false);

  // MVC staff
  model.register_observer(this);

  // Setup keyboard mapping
  QList<QPushButton *> buttons = ui->Keyboard->findChildren<QPushButton *>();
  QSignalMapper *signalMapper = new QSignalMapper(this);
  for (auto b : buttons) {
    b->setFocusPolicy(Qt::NoFocus);
    signalMapper->setMapping(b, b->text());
    connect(b, SIGNAL(clicked()), signalMapper, SLOT(map()));
  }
  connect(signalMapper, SIGNAL(mapped(const QString &)), this,
          SLOT(on_buttonPressed(const QString &)));

  // Setup X value validator
  QRegExp rx("([-+]?[0-9]*[.,]?[0-9]+(?:[eE][-+]?[0-9]+)?)");
  QValidator *xValidator = new QRegExpValidator(rx, this);
  ui->X_Value->setValidator(xValidator);

  // Setup graph
  ui->graph_area->xAxis->setRange(this->m_data.MINX, this->m_data.MAXX);
  ui->graph_area->yAxis->setRange(this->m_data.MINY, this->m_data.MAXY);
  ui->graph_area->addGraph();
}

QtGraphCalcView::~QtGraphCalcView() { delete ui; }

void QtGraphCalcView::on_graph_area_customContextMenuRequested(
    const QPoint &pos) {
  Q_UNUSED(pos);
  ui->spinBox_MAXX->setValue(this->m_data.MAXX);
  ui->spinBox_MINX->setValue(this->m_data.MINX);
  ui->spinBox_MAXY->setValue(this->m_data.MAXY);
  ui->spinBox_MINY->setValue(this->m_data.MINY);
  showSizeDialog(true);
}

void QtGraphCalcView::on_buttonBox_accepted() {
  this->m_data.MAXX = ui->spinBox_MAXX->value();
  this->m_data.MINX = ui->spinBox_MINX->value();
  this->m_data.MAXY = ui->spinBox_MAXY->value();
  this->m_data.MINY = ui->spinBox_MINY->value();

  ui->graph_area->xAxis->setRange(this->m_data.MINX, this->m_data.MAXX);
  ui->graph_area->yAxis->setRange(this->m_data.MINY, this->m_data.MAXY);
  showSizeDialog(false);
  if (ui->graph_area->graph(0)->data()->size()) {
    try {
      setupGeometry();
      controller.user_action(&m_data);
    } catch (std::invalid_argument &e) {
      ui->graph_area->replot();
    }
  } else {
    ui->graph_area->replot();
  }
}

void QtGraphCalcView::on_buttonBox_rejected() { showSizeDialog(false); }

void QtGraphCalcView::on_buttonPressed(const QString &str) {
  if (str == QString("AC")) {
    ui->InputStr->clear();
  } else if (str == QString("Bksp")) {
    ui->InputStr->backspace();
  } else if (str.length() > 1 && str != QString("mod")) {
    ui->InputStr->insert(str + QString("()"));
    ui->InputStr->cursorBackward(false);
  } else if (str == QString("=")) {
    m_data.str = ui->InputStr->text().replace(',', ".").toStdString();
    emit resultRequested();
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
  setupGraphData();
  ui->graph_area->replot();
};

void QtGraphCalcView::on_QtGraphCalcView_resultRequested() {
  try {
    setupGeometry();
    ui->graph_area->legend->setVisible(true);
    controller.user_action(&m_data);
    // make history record
    ui->listWidget->insertItem(
        0, ui->InputStr->text() + " = " + QString::number(m_data.y, 'g', 7));
    ui->listWidget->item(0)->setTextAlignment(Qt::AlignRight);
    ui->listWidget->setCurrentItem(ui->listWidget->item(0));
    ui->InputStr->setText("");
    emit showStatus(QString::number(m_data.y, 'g', 7).toStdString());
  } catch (std::invalid_argument &e) {
    emit showStatus(e.what());
  }
}

void QtGraphCalcView::on_HistoryItemDblClicked(QListWidgetItem *history) {
  std::string str = history->text().toStdString();
  ui->InputStr->setText(QString(str.substr(0, str.find(" = ")).c_str()));
}

void QtGraphCalcView::on_X_Value_textChanged(const QString &arg1) {
  QString replace_comma = arg1;
  m_data.x = replace_comma.replace(',', ".").toDouble();
  emit showStatus(std::string("X = ") + std::to_string(m_data.x));
}

void QtGraphCalcView::setupGraphData() {
  size_t n_points = m_data.y_vect.size();
  if (n_points) {
    ui->graph_area->graph(0)->data()->clear();
    QVector<double> x(m_data.x_vect.begin(), m_data.x_vect.end());
    QVector<double> y(m_data.y_vect.begin(), m_data.y_vect.end());
    ui->graph_area->graph(0)->setData(x, y);
  }
}

void QtGraphCalcView::setupGeometry() {
  m_data.clip_x1 = ui->graph_area->geometry().left();
  m_data.clip_x2 = ui->graph_area->geometry().right();
  m_data.clip_y1 = ui->graph_area->geometry().bottom();
  m_data.clip_y2 = ui->graph_area->geometry().top();
  m_data.dx = (double)(m_data.MAXX - m_data.MINX) /
              (m_data.clip_x2 - m_data.clip_x1);  //! To be or not to be
  m_data.dy = 1. / ((double)(-m_data.clip_y2 + m_data.clip_y1) /
                    (m_data.MAXY - m_data.MINY));
  ui->graph_area->graph(0)->setName(QString(m_data.str.substr(0, 17).c_str()) +
                                    ((m_data.str.size() > 17) ? "..." : "") +
                                    "\nScale x=" + QString::number(m_data.dx) +
                                    "\nScale y=" + QString::number(m_data.dy));
}

void QtGraphCalcView::resizeEvent(QResizeEvent *event) {
  QWidget::resizeEvent(event);
  setupGeometry();
}
