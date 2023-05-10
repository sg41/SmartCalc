#include "qtcreditcalcview.h"
#include "ui_qtcreditcalcview.h"

QtCreditCalcView::QtCreditCalcView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QtCreditCalcView)
{
    ui->setupUi(this);
}

QtCreditCalcView::~QtCreditCalcView()
{
    delete ui;
}
