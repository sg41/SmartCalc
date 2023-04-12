#include <gtest/gtest.h>

#include "../models/graphModel.h"

TEST(CalcTest, model) {
  CalcObserver<GraphModelData> o;
  GraphModel m1;
  m1.register_observer(o);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}