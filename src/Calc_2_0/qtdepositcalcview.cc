#include "qtdepositcalcview.h"
#include "ui_qtdepositcalcview.h"

QtDepositCalcView::QtDepositCalcView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QtDepositCalcView)
{
    ui->setupUi(this);
}

QtDepositCalcView::~QtDepositCalcView()
{
    delete ui;
}
