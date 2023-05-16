#include "qtdepositcalcview.h"

#include "ui_qtdepositcalcview.h"

QtDepositCalcView::QtDepositCalcView(QWidget *parent)
    : QWidget(parent),
      controller((BaseModel *)&model),
      ui(new Ui::QtDepositCalcView) {
  ui->setupUi(this);
  // MVC staff
  model.register_observer(this);

  // Setup sliders & spin boxes
  // Copy default values before connecting
  ui->depositTermSlider->setValue(ui->depositTermSpinBox->value());
  ui->depositAmountSlider->setValue(ui->depositAmountSpinBox->value());
  ui->interestRateSlider->setValue(ui->interestRateSpinBox->value());
  ui->taxRateSlider->setValue(ui->taxRateSpinBox->value());
  // Connect sliders and spins together
  connect(ui->depositTermSpinBox, SIGNAL(valueChanged(int)),
          ui->depositTermSlider, SLOT(setValue(int)));
  connect(ui->depositTermSlider, SIGNAL(valueChanged(int)),
          ui->depositTermSpinBox, SLOT(setValue(int)));
  connect(ui->depositAmountSpinBox, SIGNAL(valueChanged(double)),
          ui->depositAmountSlider, SLOT(doubleSetValue(double)));
  connect(ui->depositAmountSlider, SIGNAL(doubleValueChanged(double)),
          ui->depositAmountSpinBox, SLOT(setValue(double)));
  connect(ui->interestRateSpinBox, SIGNAL(valueChanged(double)),
          ui->interestRateSlider, SLOT(doubleSetValue(double)));
  connect(ui->interestRateSlider, SIGNAL(doubleValueChanged(double)),
          ui->interestRateSpinBox, SLOT(setValue(double)));
  connect(ui->taxRateSpinBox, SIGNAL(valueChanged(double)), ui->taxRateSlider,
          SLOT(doubleSetValue(double)));
  connect(ui->taxRateSlider, SIGNAL(doubleValueChanged(double)),
          ui->taxRateSpinBox, SLOT(setValue(double)));
}

QtDepositCalcView::~QtDepositCalcView() { delete ui; }

void QtDepositCalcView::observer_update(const DepositModelData *model_data) {
  m_data = *(model_data);
  QLocale rus(QLocale::Russian, QLocale::Russia);
  rus.setNumberOptions(QLocale::DefaultNumberOptions);
  ui->taxPaidLabel->setText(rus.toCurrencyString(m_data.tax));
  ui->interestAccruedLabel->setText(rus.toCurrencyString(m_data.interest));
  ui->totalDepositLabel->setText(rus.toCurrencyString(m_data.total_payment));
  ui->resultFrame->setEnabled(true);
}

void QtDepositCalcView::on_calculateButton_clicked() {
  m_data.pay_period = payPeriods[ui->paymentPeriodComboBox->currentText()];
  m_data.amount = ui->depositAmountSpinBox->value();
  m_data.duration = ui->depositTermSpinBox->value();
  m_data.rate = ui->interestRateSpinBox->value();
  m_data.tax_rate = ui->taxRateSpinBox->value();
  m_data.int_cap = ui->interestCapCheckBox->isChecked();
  m_data.withdrawal = getAnyListSum(ui->withdrawalList);
  m_data.replenishment = getAnyListSum(ui->replenishmentList);
  try {
    controller.userAction((BaseCalcData *)&m_data);
    emit showStatus("Success");
  } catch (std::invalid_argument &e) {
    ui->resultFrame->setEnabled(false);
    emit showStatus(e.what());
  }
};

double QtDepositCalcView::getAnyListSum(QListWidget *l) {
  QLocale rus(QLocale::Russian, QLocale::Russia);
  rus.setNumberOptions(QLocale::DefaultNumberOptions);
  double sum = 0;
  for (auto item : l->findItems("*", Qt::MatchWildcard)) {
    sum += rus.toDouble(item->text().left(item->text().size() - 2));
  }
  return sum;
}

void QtDepositCalcView::execCreateItemDialog(QListWidget *l,
                                             const QString &title,
                                             double maxBase) {
  QLocale rus(QLocale::Russian, QLocale::Russia);
  rus.setNumberOptions(QLocale::DefaultNumberOptions);
  DepositChangeDialog dialog(this);
  dialog.setWindowTitle(title);
  double sum = getAnyListSum(l);
  dialog.setMaxAmount(maxBase - sum);
  if (dialog.exec() == QDialog::Accepted && dialog.getAmount() > 0) {
    l->addItem(rus.toCurrencyString(dialog.getAmount()));
  }
}

void QtDepositCalcView::on_withdrawalList_customContextMenuRequested(
    const QPoint &pos) {
  Q_UNUSED(pos);
  execCreateItemDialog(ui->withdrawalList, "Add new withdrowal",
                       ui->depositAmountSpinBox->value());
}

void QtDepositCalcView::on_replenishmentList_customContextMenuRequested(
    const QPoint &pos) {
  Q_UNUSED(pos);
  execCreateItemDialog(
      ui->replenishmentList, "Add new replenishment",
      ui->depositAmountSpinBox->maximum() - ui->depositAmountSpinBox->value());
}

void QtDepositCalcView::on_QtDepositCalcView_anyList_itemDoubleClicked(
    QListWidgetItem *item) {
  delete item->listWidget()->takeItem(item->listWidget()->currentRow());
}
