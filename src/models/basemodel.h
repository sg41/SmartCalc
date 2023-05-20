#ifndef _SRC_MODELS_BASEMODEL_H_
#define _SRC_MODELS_BASEMODEL_H_
#include <list>
#include <string>
namespace s21 {
struct BaseCalcData {
  BaseCalcData() { this->initData(); };
  virtual ~BaseCalcData(){};
  virtual void initData() { this->error = 0; };
  static const int kMaxStr = 1024;
  static const double constexpr kEpsilon = 1e-7;
  std::string error_message;
  int error;
};

template <class D>
class ModelObserverInterface {
 public:
  virtual void observerUpdate(const D *) = 0;
};

template <class D>
class ModelObservableInterface {
 public:
  void registerObserver(ModelObserverInterface<D> *o) {
    observers_.push_back(o);
  };
  void removeObserver(ModelObserverInterface<D> *o) { observers_.remove(o); };
  void notifyObservers() {
    if (observers_.size() > 0)
      for (auto o : observers_) o->observerUpdate(getData());
  };
  virtual const D *getData() const = 0;

 protected:
  std::list<ModelObserverInterface<D> *> observers_;
};

template <class D>
class AbstractModel : public ModelObservableInterface<D> {
 public:
  AbstractModel() : data_(new D){};
  AbstractModel(const AbstractModel &m) : data_(new D(*m.data_)){};
  AbstractModel(AbstractModel &&m) {
    data_ = m.data_;
    m.data_ = nullptr;
  };
  AbstractModel &operator=(const AbstractModel &m) {
    if (this != &m) setData(m.data_);
    return *this;
  }
  AbstractModel &operator=(AbstractModel &&m) {
    if (this != &m) {
      delete data_;
      data_ = m.data_;
      m.data_ = nullptr;
    }
    return *this;
  }
  virtual ~AbstractModel() { delete data_; };
  virtual void calculate() = 0;
  int validateData() { return data_->validateData(); };
  virtual void setData(const D *d) {
    if (d != nullptr) *data_ = *d;
  };
  const D *getData() const override { return static_cast<const D *>(data_); };
  void clearData() { data_->initData(); };

 protected:
  D *data_ = nullptr;
};

}  // namespace s21
#endif  // _SRC_MODELS_BASEMODEL_H_
