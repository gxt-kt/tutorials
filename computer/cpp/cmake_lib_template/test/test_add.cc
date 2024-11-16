#include <gtest/gtest.h>

#include "mytest.h"

TEST(test_add, test_add1) { ASSERT_EQ(TestAdd(10, 20), 30); }
TEST(test_add, test_add2) { ASSERT_EQ(TestAdd(-10, 10), 0); }
