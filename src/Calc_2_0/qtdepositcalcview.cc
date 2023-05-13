#include "qtdepositcalcview.h"

#include "ui_qtdepositcalcview.h"

QtDepositCalcView::QtDepositCalcView(QWidget *parent)
    : QWidget(parent),
      controller((BaseModel *)&model),
      ui(new Ui::QtDepositCalcView) {
  ui->setupUi(this);
  // MVC staff
  model.register_observer(this);
}

QtDepositCalcView::~QtDepositCalcView() { delete ui; }

void QtDepositCalcView::observer_update(const DepositModelData *model_data) {
  //  m_data = *(model_data);
  //  QLocale rus(QLocale::Russian, QLocale::Russia);
  //  rus.setNumberOptions(QLocale::DefaultNumberOptions);
  //  ui->monthlyPaymentLabel->setText(
  //      rus.toCurrencyString(m_data.monthly_payment));
  //  ui->overPaymentLabel->setText(rus.toCurrencyString(m_data.overpayment));
  //  ui->totalPaymentLabel->setText(rus.toCurrencyString(m_data.total_payment));
  //  ui->resultFrame->setEnabled(true);
}
