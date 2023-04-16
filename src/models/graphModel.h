#ifndef _GRAPHMODEL_H_
#define _GRAPHMODEL_H_

#include <vector>

#include "baseModel.h"

class GraphModelData : public BaseCalcData {
 public:
  using BaseCalcData::BaseCalcData;
  GraphModelData() { init_data(); };
  double clip_x1() const { return clip_x1_; };
  double clip_y1() const { return clip_y1_; };
  double clip_x2() const { return clip_x2_; };
  double clip_y2() const { return clip_y1_; };

 public:
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

  void init_data() override {
    BaseCalcData::init_data();
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

class GraphModel : public BaseCalcModel<GraphModelData> {
 public:
  // GraphModel(){};
  using BaseCalcModel<GraphModelData>::BaseCalcModel;
  int validate_data(/*GraphModelData *d*/) override {
    int err = 0;
    if (data->clip_x1() >= BaseCalcData::VERY_MIN_X &&
        data->clip_y1() >= BaseCalcData::VERY_MIN_Y &&
        data->clip_x2() <= BaseCalcData::VERY_MAX_X &&
        data->clip_y2() <= BaseCalcData::VERY_MAX_Y)
      err = 0;
    else
      err = 1;
    return err;
  };
};

#endif  //_GRAPHMODEL_H_