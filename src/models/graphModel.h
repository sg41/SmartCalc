#ifndef _GRAPHMODEL_H_
#define _GRAPHMODEL_H_

#include <vector>

#include "../rpn_cpp/core.h"
#include "baseModel.h"

namespace s21 {
class GraphModelData : public BaseCalcData {
 public:
  using BaseCalcData::BaseCalcData;
  GraphModelData() { GraphModelData::init_data(); };
  int validate_data() {
    int err = 0;
    if (MINX >= VERY_MIN_X && MINY >= VERY_MIN_Y && MAXX <= VERY_MAX_X &&
        MAXY <= VERY_MAX_Y)
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
    clip_x2 = 0;
    clip_y1 = 0;
    clip_y2 = 0;
    dx = (double)(MAXX - MINX) / MAXI;  //! To be or not to be
    dy = (double)MAXJ / (MAXY - MINY);
    y = 0;
    y_vect.clear();
    x_vect.clear();
  };
};

class GraphModel : public AbstractModel<GraphModelData> {
 public:
  using AbstractModel<GraphModelData>::AbstractModel;

  void calculate() override {
    data->y = c.calc(data->x);  // calculate single Y for given X
    data->y_vect.clear();       // calculate Y vector for X range
    data->x_vect.clear();
    if (data->dx > 0) {
      double visible_area = (data->MAXY - data->MINY);
      for (double x = data->MINX; x < data->MAXX; x += data->dx) {
        data->x_vect.push_back(x);
        double res = c.calc(x);
        if (!data->y_vect.empty() &&
            (fabs(data->y_vect.back() - res) > visible_area &&
             (res * data->y_vect.back()) < 0)) {
          data->y_vect.push_back(std::numeric_limits<double>::quiet_NaN());
        } else {
          data->y_vect.push_back((res < GraphModelData::VERY_MAX_Y &&
                                  res > GraphModelData::VERY_MIN_Y)
                                     ? res
                                     : std::numeric_limits<double>::infinity());
        }
      }
      notify_observers();
    }
  };

  void set_data(const GraphModelData *d) override {
    AbstractModel::set_data(d);
    c.makeRpnExpr(data->str);
  };

 protected:
  CalcCore c;
};
}  // namespace s21
#endif  //_GRAPHMODEL_H_