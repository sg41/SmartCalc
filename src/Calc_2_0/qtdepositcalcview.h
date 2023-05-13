#ifndef QTDEPOSITCALCVIEW_H
#define QTDEPOSITCALCVIEW_H

#include <QWidget>

#include "../controllers/exampleController.h"
#include "../models/depositModel.h"
using namespace s21;

namespace Ui {
class QtDepositCalcView;
}

class QtDepositCalcView : public QWidget,
                          public ModelObserverInterface<DepositModelData> {
  Q_OBJECT

 public:
  explicit QtDepositCalcView(QWidget *parent = nullptr);
  ~QtDepositCalcView();
  // Observer staff
  void observer_update(const DepositModelData *model_data) override;
  DepositModelData m_data;

 private:
  // MVC staff
  DepositModel model;
  ExampleController controller;
  Ui::QtDepositCalcView *ui;
};

#endif  // QTDEPOSITCALCVIEW_H
