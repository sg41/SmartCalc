#ifndef _GRAPHMODEL_H_
#define _GRAPHMODEL_H_

#include <vector>

#include "calcModel.h"

class GraphModelData : public CalcData {
 public:
  GraphModelData(){};
  double clip_x1() const { return clip_x1_; };
  double clip_y1() const { return clip_y1_; };
  double clip_x2() const { return clip_x2_; };
  double clip_y2() const { return clip_y1_; };

 protected:
  int iteration;
  std::string str;
  double x;
  double clip_x1_;
  double clip_y1_;
  double clip_x2_;
  double clip_y2_;
  double dx;
  double dy;
  std::vector<double> y;

  void init_data() {
    CalcData::init_data();
    x = 0;
    clip_x1_ = 0;
    clip_y1_ = 0;
    clip_x2_ = 0;
    clip_y2_ = 0;
    dx = 0;
    dy = 0;
    y.clear();
  };
};

class GraphModel : public CalcModel<GraphModelData> {
 public:
  GraphModel(){};
  int validate_data(GraphModelData &d) override {
    int err = 0;
    if (d.clip_x1() >= CalcData::VERY_MIN_X &&
        d.clip_y1() >= CalcData::VERY_MIN_Y &&
        d.clip_x2() <= CalcData::VERY_MAX_X &&
        d.clip_y2() <= CalcData::VERY_MAX_Y)
      err = 0;
    else
      err = 1;
    return err;
  };
};

#endif  //_GRAPHMODEL_H_