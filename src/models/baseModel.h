#ifndef _CALCDATA_H_
#define _CALCDATA_H_
#include <list>
#include <string>

class BaseCalcData {
 public:
  BaseCalcData() { this->init_data(); };
  virtual ~BaseCalcData(){};
  virtual void init_data() { this->error = 0; };
  virtual int validate_data() { return 0; };

 public:
  static const int MAXSTR = 1024;
  static const int SECOND_PER_DAY = 86400;
  static const int ANNUITET = 0;
  static const int DIFFERENTIATED = 1;
  static const int DEFAULT_PAY_PERIOD = 30;
  static const int DEFAULT_DURATION = 12;
  static const int DEFAULT_AMOUNT = 100000;
  static const int DEFAULT_RATE = 9.5;
  int MINX = -3;
  int MAXX = 3;  // 4 * 3.14
  int MINY = -1;
  int MAXY = 1;
  static const int VERY_MIN_X = -1000000;
  static const int VERY_MAX_X = 1000000;
  static const int VERY_MIN_Y = -1000000;
  static const int VERY_MAX_Y = 1000000;
  static const int EPS = 1e-7;
  int MAXI = 80;
  int MAXJ = 25;

 public:
  std::string error_message;
  int error;
};

template <class D>
class ModelObserverInterface {
 public:
  virtual void update(const D *){};

 protected:
};

template <class D>
class ModelObservableInterface {
 public:
  void register_observer(ModelObserverInterface<D> *o) {
    observers.push_back(o);
  };
  void remove_observer(ModelObserverInterface<D> *o) { observers.remove(o); };
  void notify_observers() {
    if (observers.size() > 0)
      for (auto o : observers) o->update(get_data());
  };
  virtual const D *get_data() const = 0;

 protected:
  std::list<ModelObserverInterface<D> *> observers;
};

template <class D>
class BaseCalcModel : public ModelObservableInterface<D> {
 public:
  BaseCalcModel() { data = new D; };
  BaseCalcModel(const BaseCalcModel &m) { data = new D(*m.data); };
  BaseCalcModel(BaseCalcModel &&m) {
    data = m.data;
    m.data = nullptr;
  };
  BaseCalcModel &operator=(const BaseCalcModel &m) {
    if (this != &m) set_data(m.data);
    return *this;
  }
  BaseCalcModel &operator=(BaseCalcModel &&m) {
    if (this != &m) {
      if (data != nullptr) delete data;
      data = m.data;
      m.data = nullptr;
    }
    return *this;
  }
  virtual ~BaseCalcModel() { delete data; };
  virtual void calculate(){};
  int validate_data() { return data->validate_data(); };
  virtual void set_data(const D *d) {
    if (data == nullptr) data = new D;
    if (d != nullptr) *data = *d;
  };
  const D *get_data() const override { return static_cast<const D *>(data); };
  void clear_data() { data->init_data(); };

 protected:
  D *data = nullptr;
};

#endif  // _CALCDATA_H
