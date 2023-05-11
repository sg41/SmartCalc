#include "qtcreditcalcview.h"

#include "ui_qtcreditcalcview.h"

QtCreditCalcView::QtCreditCalcView(QWidget *parent)
    : QWidget(parent),
      controller((BaseModel *)&model),
      ui(new Ui::QtCreditCalcView) {
  ui->setupUi(this);

  // MVC staff
  model.register_observer(this);

  // Setup sliders & spin boxes
  // Copy default values before connecting
  ui->creditTermSlider->setValue(ui->creditTermSpinBox->value());
  ui->creditAmountSlider->setValue(ui->creditAmountSpinBox->value());
  ui->interestRateSlider->setValue(ui->interestRateSpinBox->value());
  // Connect sliders and spins together
  connect(ui->creditTermSpinBox, SIGNAL(valueChanged(int)),
          ui->creditTermSlider, SLOT(setValue(int)));
  connect(ui->creditTermSlider, SIGNAL(valueChanged(int)),
          ui->creditTermSpinBox, SLOT(setValue(int)));
  connect(ui->creditAmountSpinBox, SIGNAL(valueChanged(double)),
          ui->creditAmountSlider, SLOT(doubleSetValue(double)));
  connect(ui->creditAmountSlider, SIGNAL(doubleValueChanged(double)),
          ui->creditAmountSpinBox, SLOT(setValue(double)));
  connect(ui->interestRateSpinBox, SIGNAL(valueChanged(double)),
          ui->interestRateSlider, SLOT(doubleSetValue(double)));
  connect(ui->interestRateSlider, SIGNAL(doubleValueChanged(double)),
          ui->interestRateSpinBox, SLOT(setValue(double)));
}

QtCreditCalcView::~QtCreditCalcView() { delete ui; }

void QtCreditCalcView::observer_update(const CreditModelData *model_data) {
  m_data = *(model_data);
  ui->monthlyPaymentLabel->setText(QString::number(m_data.monthly_payment));
  ui->overPaymentLabel->setText(QString::number(m_data.overpayment));
  ui->totalPaymentLabel->setText(QString::number(m_data.total_payment));
  ui->resultFrame->setEnabled(true);
}

void QtCreditCalcView::on_calculateButton_pressed() {
  m_data.type = ui->creditTypeComboBox->currentText() == "Annuitet"
                    ? m_data.ANNUITET
                    : m_data.DIFFERENTIATED;
  m_data.amount = ui->creditAmountSpinBox->value();
  m_data.duration = ui->creditTermSpinBox->value();
  m_data.rate = ui->interestRateSpinBox->value();
  m_data.round = ui->round->isChecked();
  try {
    controller.user_action((BaseCalcData *)&m_data);
    emit showStatus("Success");
  } catch (std::invalid_argument &e) {
    ui->resultFrame->setEnabled(false);
    emit showStatus(e.what());
  }
}
