#ifndef QTGRAPHCALCVIEW_H
#define QTGRAPHCALCVIEW_H

#include <QWidget>

#include "../controllers/examplecontroller.h"
#include "../models/graphmodel.h"
#include "QCP/qcustomplot.h"
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
  void observerUpdate(const GraphModelData *model_data) override;
  GraphModelData m_data;
  static const int kMaxLegendTextLen = 17;

 private slots:
  void on_graph_area_customContextMenuRequested(const QPoint &pos);

  void on_buttonBox_accepted();

  void on_buttonBox_rejected();

  void on_buttonPressed(const QString &str);

  void on_QtGraphCalcView_resultRequested();

  void on_HistoryItemDblClicked(QListWidgetItem *);

  void on_X_Value_textChanged(const QString &arg1);

  void showSizeDialog(bool on);

  void setupGraphData();

  void setupGeometry();

  void resizeEvent(QResizeEvent *event) override;

 signals:
  void resultRequested();
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
