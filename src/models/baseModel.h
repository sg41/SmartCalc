#ifndef _CALCDATA_H_
#define _CALCDATA_H_
#include <list>
#include <string>

class BaseCalcData {
 public:
  BaseCalcData() { this->init_data(); };
  virtual void init_data() { this->error = 0; };
  void set_error(int e) { this->error = e; };
  void set_error(int e, std::string errMessage) {
    set_error(e);
    this->error_message = errMessage;
  };

 public:
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

 protected:
  std::string error_message;
  int error;
};

template <class D>
class ModelObservableInterface;

template <class D>
class CalcObserverInterface {
 public:
  virtual void update(ModelObservableInterface<D> *){};
};

template <class D>
class ModelObservableInterface {
 public:
  void register_observer(CalcObserverInterface<D> *o) {
    observers.push_back(o);
  };
  void remove_observer(CalcObserverInterface<D> *o) { observers.remove(o); };
  void notify_observers() {
    for (auto o : observers) o->update(this);
  };
  virtual const D &get_data() const = 0;

 protected:
  std::list<CalcObserverInterface<D> *> observers;
};

template <class D>
class BaseCalcModel : public ModelObservableInterface<D> {
 public:
  using ModelObservableInterface<D>::ModelObservableInterface;
  virtual void calculate(){};
  virtual int validate_data(D &d) = 0;

  void set_data(D &d) { data = d; };
  const D &get_data() const override { return (const D &)data; };
  void clear_data() { data.init_data(); };

 protected:
  D data;
};

#endif  // _CALCDATA_H