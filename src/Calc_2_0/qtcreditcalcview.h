#ifndef QTCREDITCALCVIEW_H
#define QTCREDITCALCVIEW_H

#include <QWidget>

namespace Ui {
class QtCreditCalcView;
}

class QtCreditCalcView : public QWidget
{
    Q_OBJECT

public:
    explicit QtCreditCalcView(QWidget *parent = nullptr);
    ~QtCreditCalcView();

private:
    Ui::QtCreditCalcView *ui;
};

#endif // QTCREDITCALCVIEW_H
