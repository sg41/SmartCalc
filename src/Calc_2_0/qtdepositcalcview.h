#ifndef QTDEPOSITCALCVIEW_H
#define QTDEPOSITCALCVIEW_H

#include <QWidget>

#include "../controllers/exampleController.h"
#include "../models/depositModel.h"
using namespace s21;

namespace Ui {
class QtDepositCalcView;
}

class QtDepositCalcView : public QWidget

{
  Q_OBJECT

 public:
  explicit QtDepositCalcView(QWidget *parent = nullptr);
  ~QtDepositCalcView();

 private:
  Ui::QtDepositCalcView *ui;
};

#endif  // QTDEPOSITCALCVIEW_H
