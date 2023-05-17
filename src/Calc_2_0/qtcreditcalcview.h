#ifndef QTCREDITCALCVIEW_H
#define QTCREDITCALCVIEW_H

#include <QWidget>

#include "../controllers/examplecontroller.h"
#include "../models/creditmodel.h"
using namespace s21;

namespace Ui {
class QtCreditCalcView;
}

class QtCreditCalcView : public QWidget,
                         public ModelObserverInterface<CreditModelData> {
  Q_OBJECT

 public:
  explicit QtCreditCalcView(QWidget *parent = nullptr);
  ~QtCreditCalcView();
  // Observer staff
  void observerUpdate(const CreditModelData *model_data) override;
  CreditModelData m_data;

 private slots:
  void on_calculateButton_pressed();

 signals:
  void showStatus(std::string);

 private:
  // MVC staff
  CreditModel model;
  ExampleController controller;
  // Default staff
  Ui::QtCreditCalcView *ui;
};

#endif  // QTCREDITCALCVIEW_H
