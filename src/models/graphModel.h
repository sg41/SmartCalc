#ifndef _GRAPHMODEL_H_
#define _GRAPHMODEL_H_

#include <vector>

#include "baseModel.h"

class GraphModelData : public BaseCalcData {
 public:
  using BaseCalcData::BaseCalcData;
  GraphModelData() { init_data(); };
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
  std::vector<double> y;

  void init_data() override {
    BaseCalcData::init_data();
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

class GraphModel : public BaseCalcModel<GraphModelData> {
 public:
  using BaseCalcModel<GraphModelData>::BaseCalcModel;
  void calculate() override{

  };
};

#endif  //_GRAPHMODEL_H_