#ifndef QTGRAPHCALCVIEW_H
#define QTGRAPHCALCVIEW_H

#include <QWidget>

#include "../controllers/exampleController.h"
#include "../models/graphModel.h"
#include "qcustomplot.h"  // the header file of QCustomPlot. Don't forget to add it to your project, if you use an IDE, so it gets compiled.
using namespace s21;

QT_BEGIN_NAMESPACE
namespace Ui {
class QtGraphCalcView;
}
QT_END_NAMESPACE

class QtGraphCalcView : public QWidget,
                        public ModelObserverInterface<GraphModelData>

{
  Q_OBJECT

 public:
  explicit QtGraphCalcView(QWidget *parent = nullptr);
  ~QtGraphCalcView();

  // Observer staff
  void observer_update(const GraphModelData *model_data) override;
  GraphModelData m_data;

 private slots:
  void on_graph_area_customContextMenuRequested(const QPoint &pos);

  void on_buttonBox_accepted();

  void on_buttonBox_rejected();

  void buttonPressed(const QString &str);

  void showSizeDialog(bool on);

  void on_QtGraphCalcView_result_requested();

  void onHistoryItemDblClicked(QListWidgetItem *);

  void on_X_Value_textChanged(const QString &arg1);

  void setup_graph_data();

  void setup_geometry();

  void resizeEvent(QResizeEvent *event) override;

 signals:
  void result_requested();
  void showStatus(std::string);

 private:
  // MVC staff
  GraphModel model;
  ExampleController controller;
  // Customplot staff
  QVector<double> x_points, y_points;
  // Default staff
  Ui::QtGraphCalcView *ui;
};

#endif  // QTGRAPHCALCVIEW_H
