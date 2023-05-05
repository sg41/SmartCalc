#ifndef _GRAPHMODEL_H_
#define _GRAPHMODEL_H_

#include <vector>

#include "../rpn_cpp/core.h"
#include "baseModel.h"

class GraphModelData : public BaseCalcData {
 public:
  using BaseCalcData::BaseCalcData;
  GraphModelData() { GraphModelData::init_data(); };
  int validate_data() override {
    int err = 0;
    if (clip_x1 >= VERY_MIN_X && clip_y1 >= VERY_MIN_Y &&
        clip_x2 <= VERY_MAX_X && clip_y2 <= VERY_MAX_Y)
      err = 0;
    else
      err = 1;
    return err;
  };

 public:
  int iteration;
  std::string str;
  double x;
  double clip_x1;
  double clip_y1;
  double clip_x2;
  double clip_y2;
  double dx;
  double dy;
  std::vector<double> x_vect, y_vect;
  double y;

  void init_data() override {
    BaseCalcData::init_data();
    x = 0;
    clip_x1 = 0;
    clip_y1 = 0;
    clip_x2 = 0;
    clip_y2 = 0;
    dx = (double)(MAXX - MINX) / MAXI;  //! To be or not to be
    dy = (double)MAXJ / (MAXY - MINY);
    y = 0;
    y_vect.clear();
    x_vect.clear();
  };
};

class GraphModel : public BaseCalcModel<GraphModelData> {
 public:
  using BaseCalcModel<GraphModelData>::BaseCalcModel;
  void calculate() override {
    data->y = c.calc(data->x);  // calculate single Y for given X
    data->y_vect.clear();       // calculate Y vector for X range
    data->x_vect.clear();
    double x = data->MINX;
    if (data->dx != 0) {
      for (; x < data->MAXX; x += data->dx) {
        data->x_vect.push_back(x);
        data->y_vect.push_back(c.calc(x));
      }
      notify_observers();
    }
  }
  void set_data(const GraphModelData *d) override {
    BaseCalcModel::set_data(d);
    c.make_rpn_expr(data->str);
  };

 protected:
  CalcCore c;
};

#endif  //_GRAPHMODEL_H_