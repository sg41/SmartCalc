#include <gtest/gtest.h>

#include "../models/graphModel.h"

class TestObserver : public CalcObserverInterface<GraphModelData> {
 public:
  void update(ModelObservableInterface<GraphModelData> *d) override {
    std::cout << "Updated" << std::endl;
    std::cout << d->get_data().clip_x1() << std::endl;
  };
};

TEST(CalcTest, model) {
  TestObserver o;
  GraphModel m1;
  m1.register_observer(&o);
  m1.notify_observers();
  m1.remove_observer(&o);
  m1.notify_observers();
  // EXPECT_EQ()
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}