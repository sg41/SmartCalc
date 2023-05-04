#ifndef QTGRAPHCALCVIEW_H
#define QTGRAPHCALCVIEW_H

#include <QWidget>

#include "../controllers/exampleController.h"
#include "../models/graphModel.h"
#include "qcustomplot.h"  // the header file of QCustomPlot. Don't forget to add it to your project, if you use an IDE, so it gets compiled.

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

 signals:
  void result_requested();
  void inputError(std::string);

 private:
  // MVC staff
  GraphModel model;
  ExampleController controller;
  // Child objects
  QSignalMapper *signalMapper;
  // Default staff
  Ui::QtGraphCalcView *ui;
};

#endif  // QTGRAPHCALCVIEW_H
