#ifndef MYQTDOUBLESLIDER_H
#define MYQTDOUBLESLIDER_H

#include <QSlider>

class MyQtDoubleSlider : public QSlider {
  Q_OBJECT
 public:
  MyQtDoubleSlider(QWidget *parent = 0) : QSlider(parent) {
    connect(this, SIGNAL(valueChanged(int)), this,
            SLOT(convertValueChanged(int)));
  };

 signals:
  void doubleValueChanged(double value);

 public slots:
  void convertValueChanged(int value) {
    emit doubleValueChanged((double)value);
  };
  void doubleSetValue(double value) { emit setValue((int)value); };
};

#endif  // MYQTDOUBLESLIDER_H
