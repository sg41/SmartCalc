#include "qtgraphcalcview.h"

QtGraphCalcView::QtGraphCalcView() {}

void QtGraphCalcView::observer_update(const GraphModelData *model_data) {
  m_data = *(model_data);
  if (!graph_area) {
    graph_area = findChild<QCustomPlot *>(QString("graph_area"));
    if (!graph_area) throw std::invalid_argument("graph_area is missing");
  }
  graph_area->replot();
};
