#ifndef QTGRAPHCALCVIEW_H
#define QTGRAPHCALCVIEW_H

#include <QObject>
#include <QWidget>

#include "../controllers/exampleController.h"
#include "../models/graphModel.h"
#include "qcustomplot.h"  // the header file of QCustomPlot. Don't forget to add it to your project, if you use an IDE, so it gets compiled.

class QtGraphCalcView : public QWidget,
                        public ModelObserverInterface<GraphModelData>

{
 public:
  QtGraphCalcView();
  void observer_update(const GraphModelData *model_data) override;
  GraphModelData m_data;

 protected:
  QCustomPlot *graph_area = nullptr;
  ExampleController *controller = nullptr;  // No need
};

#endif  // QTGRAPHCALCVIEW_H
