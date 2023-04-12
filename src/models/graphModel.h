#ifndef _GRAPHMODEL_H_
#define _GRAPHMODEL_H_

#include <vector>

#include "calcModel.h"

class GraphModelData : public CalcData {
 public:
  GraphModelData(){};

 protected:
  int iteration;
  std::string str;
  double x;
  double clip_x1;
  double clip_y1;
  double clip_x2;
  double clip_y2;
  double dx;
  double dy;
  std::vector<double> y;

  void init_data() {
    CalcData::init_data();
    x = 0;
    clip_x1 = 0;
    clip_y1 = 0;
    clip_x2 = 0;
    clip_y2 = 0;
    dx = 0;
    dy = 0;
    y.clear();
  };
};

class GraphModel : public CalcModel<GraphModelData> {
 public:
  GraphModel(){};
};

#endif  //_GRAPHMODEL_H_