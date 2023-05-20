#ifndef _SRC_MODELS_GRAPHMODEL_H_
#define _SRC_MODELS_GRAPHMODEL_H_

#include <cctype>
#include <limits>
#include <vector>

#include "../rpn_cpp/core.h"
#include "basemodel.h"

namespace s21 {
struct GraphModelData : public BaseCalcData {
  using BaseCalcData::BaseCalcData;
  GraphModelData() { GraphModelData::initData(); };
  int validateData() {
    int err = 0;
    if (min_x >= kVeryMinX && min_y >= kVeryMinY && max_x <= kVeryMaxX &&
        max_y <= kVeryMaxY)
      err = 0;
    else
      err = 1;
    return err;
  };
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
  int min_x = -3;
  int max_x = 3;
  int min_y = -1;
  int max_y = 1;
  int max_console_width = 80;
  int max_console_height = 25;
  static const int kVeryMinX = -1000000;
  static const int kVeryMaxX = 1000000;
  static const int kVeryMinY = -1000000;
  static const int kVeryMaxY = 1000000;

  void initData() override {
    BaseCalcData::initData();
    x = 0;
    clip_x1 = 0;
    clip_x2 = 0;
    clip_y1 = 0;
    clip_y2 = 0;
    min_x = -3;
    max_x = 3;  // 4 * 3.14
    min_y = -1;
    max_y = 1;
    max_console_width = 80;
    max_console_height = 25;
    dx = (double)(max_x - min_x) / max_console_width;  //! To be or not to be
    dy = (double)max_console_height / (max_y - min_y);
    y = 0;
    y_vect.clear();
    x_vect.clear();
  };
};

class GraphModel : public AbstractModel<GraphModelData> {
 public:
  using AbstractModel<GraphModelData>::AbstractModel;

  void calculate() override {
    data_->y = calculator_.calc(data_->x);  // calculate single Y for given X
    data_->y_vect.clear();                  // calculate Y vector for X range
    data_->x_vect.clear();
    if (data_->dx > 0) {
      double visible_area = (data_->max_y - data_->min_y);
      for (double x = data_->min_x; x < data_->max_x; x += data_->dx) {
        data_->x_vect.push_back(x);
        double res = calculator_.calc(x);
        if (!data_->y_vect.empty() &&
            (fabs(data_->y_vect.back() - res) > visible_area &&
             (res * data_->y_vect.back()) < 0)) {
          data_->y_vect.push_back(std::numeric_limits<double>::quiet_NaN());
        } else {
          data_->y_vect.push_back(
              (res < GraphModelData::kVeryMaxY &&
               res > GraphModelData::kVeryMinY)
                  ? res
                  : std::numeric_limits<double>::infinity());
        }
      }
      notifyObservers();
    }
  };

  void setData(const GraphModelData *d) override {
    AbstractModel::setData(d);
    std::string lower_case = data_->str;
    std::transform(data_->str.begin(), data_->str.end(), lower_case.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    calculator_.makeRpnExpr(lower_case);
  };

 protected:
  CalcCore calculator_;
};
}  // namespace s21
#endif  //_SRC_MODELS_GRAPHMODEL_H_
