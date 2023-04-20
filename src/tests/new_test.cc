#include <gtest/gtest.h>

#include "../models/graphModel.h"
#include "../rpn_cpp/core.h"

class TestObserver : public CalcObserverInterface<GraphModelData> {
 public:
  void update(ModelObservableInterface<GraphModelData> *d) override {
    std::cout << "Updated" << std::endl;
    std::cout << d->get_data()->clip_x1 << std::endl;
  };
};

TEST(CalcTest, model) {
  TestObserver o;
  GraphModel m1;
  m1.register_observer(&o);
  m1.notify_observers();
  m1.remove_observer(&o);
  m1.notify_observers();
  GraphModelData *d = (GraphModelData *)m1.get_data();
  EXPECT_EQ(m1.validate_data(), 0);
  d->clip_x2 = GraphModelData::VERY_MAX_X + 10;
  EXPECT_GT(d->clip_x2, GraphModelData::VERY_MAX_X + 0);
  EXPECT_NE(m1.validate_data(), 0);
  GraphModel m2(m1);
  GraphModelData *d2 = (GraphModelData *)m2.get_data();
  EXPECT_GT(d2->clip_x2, GraphModelData::VERY_MAX_X + 0);
  d2->x = 999;
  EXPECT_NE(d->x, d2->x);
}
TEST(CalcTest, calcEngine) { CalcEngine c; }
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}