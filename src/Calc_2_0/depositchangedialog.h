#ifndef DEPOSITCHANGEDIALOG_H
#define DEPOSITCHANGEDIALOG_H

#include <QDialog>

namespace Ui {
class DepositChangeDialog;
}

class DepositChangeDialog : public QDialog {
  Q_OBJECT

 public:
  explicit DepositChangeDialog(QWidget *parent = nullptr);
  ~DepositChangeDialog();
  double getAmount();
  void setMaxAmount(double m);

 private:
  Ui::DepositChangeDialog *ui;
};

#endif  // DEPOSITCHANGEDIALOG_H
