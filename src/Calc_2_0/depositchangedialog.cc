#include "depositchangedialog.h"

#include "ui_depositchangedialog.h"

DepositChangeDialog::DepositChangeDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::DepositChangeDialog) {
  ui->setupUi(this);
  connect(ui->amountSpinBox, SIGNAL(valueChanged(double)), ui->amountSlider,
          SLOT(doubleSetValue(double)));
  connect(ui->amountSlider, SIGNAL(doubleValueChanged(double)),
          ui->amountSpinBox, SLOT(setValue(double)));
}

DepositChangeDialog::~DepositChangeDialog() { delete ui; }

double DepositChangeDialog::getAmount() { return ui->amountSpinBox->value(); }

void DepositChangeDialog::setMaxAmount(double m) {
  if (m <= 0) {
    ui->amountSlider->setEnabled(false);
    ui->amountSpinBox->setEnabled(false);
    m = 0;
  }
  ui->amountSlider->setMaximum((int)m);
  ui->amountSpinBox->setMaximum(m);
}
