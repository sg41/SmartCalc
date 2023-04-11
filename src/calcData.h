#ifndef SRC_CALCDATA_H_
#define SRC_CALCDATA_H_
#include <string>

class CalcData {
 public:
  CalcData() { init_calc_data(); };

 protected:
  static const int MAXSTR = 1024;
  static const int SECOND_PER_DAY = 86400;
  static const int ANNUITET = 0;
  static const int DIFFERENTIATED = 1;
  static const int DEFAULT_PAY_PERIOD = 30;
  static const int DEFAULT_DURATION = 12;
  static const int DEFAULT_AMOUNT = 100000;
  static const int DEFAULT_RATE = 9.5;
  static const int MINX = -3;
  static const int MAXX = 3;  // 4 * 3.14
  static const int MINY = -1;
  static const int MAXY = 1;
  static const int VERY_MIN_X = -1000000;
  static const int VERY_MAX_X = 1000000;
  static const int VERY_MIN_Y = -1000000;
  static const int VERY_MAX_Y = 1000000;
  static const int EPS = 1e-7;

  void init_calc_data();
  std::string error_message;
  int error;
};

void CalcData::init_calc_data() { this->error = 0; }

#endif  // SRC_CALCDATA_H
