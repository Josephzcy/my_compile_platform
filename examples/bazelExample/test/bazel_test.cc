#include "gtest/gtest.h"
#include "bazel_component_inf.h"

TEST(testAdd,test0)
{
   EXPECT_EQ(15,add(5,10));
}