#ifndef QTDEPOSITCALCVIEW_H
#define QTDEPOSITCALCVIEW_H

#include <QListWidgetItem>
#include <QWidget>

#include "../controllers/exampleController.h"
#include "../models/depositModel.h"
#include "depositchangedialog.h"
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

 signals:
  void showStatus(std::string);
  void anyList_itemDoubleClicked(QListWidgetItem *);

 protected:
  void execCreateItemDialog(QListWidget *l, const QString &title,
                            double maxBase);

 private slots:
  void on_calculateButton_clicked();

  void on_withdrawalList_customContextMenuRequested(const QPoint &pos);

  void on_replenishmentList_customContextMenuRequested(const QPoint &pos);

  void on_QtDepositCalcView_anyList_itemDoubleClicked(QListWidgetItem *);

 private:
  std::map<QString, int> payPeriods{
      {"Daily", 1},      {"Weekly", 7},         {"Monthly", 30},
      {"Quarterly", 90}, {"Semiannually", 180}, {"Once a year", 360}};
  // MVC staff
  DepositModel model;
  ExampleController controller;
  // Default staff
  Ui::QtDepositCalcView *ui;
};

#endif  // QTDEPOSITCALCVIEW_H
