/**
 * @file	condition_tests.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/05
 */

/* -- Includes -- */

#include "test_base.hpp"

/* -- Namespaces -- */

using namespace spookshow;
using namespace testing;

/* -- Test Cases -- */

/**
 * Unit test for spookshow conditions.
 */
class ConditionTests : public ::spookshow::tests::TestBase
{
};

TEST_F(ConditionTests, ArgEqFunctor)
{
  auto cond0 = arg_eq<0>(100);
  EXPECT_EQ(cond0(100), true);
  EXPECT_EQ(cond0(101), false);
  EXPECT_EQ(cond0(100, 101), true);
  EXPECT_EQ(cond0(101, 101), false);

  auto cond1 = arg_eq<1>(200);
  EXPECT_EQ(cond1(0, 200), true);
  EXPECT_EQ(cond1(0, 201), false);
}

TEST_F(ConditionTests, ArgNeFunctor)
{
  auto cond0 = arg_ne<0>(100);
  EXPECT_EQ(cond0(100), false);
  EXPECT_EQ(cond0(101), true);
  EXPECT_EQ(cond0(100, 101), false);
  EXPECT_EQ(cond0(101, 101), true);

  auto cond1 = arg_ne<1>(200);
  EXPECT_EQ(cond1(0, 200), false);
  EXPECT_EQ(cond1(0, 201), true);
}

TEST_F(ConditionTests, AndOperator)
{
  auto cond0 = arg_eq<0>(100);
  auto cond1 = arg_ne<1>(200);
  auto cond = cond0 && cond1;

  EXPECT_EQ(cond(100, 201), true);	// T && T
  EXPECT_EQ(cond(100, 200), false);	// T && F
  EXPECT_EQ(cond(101, 201), false);	// F && T
  EXPECT_EQ(cond(101, 200), false);	// F && F
}

TEST_F(ConditionTests, OrOperator)
{
  auto cond0 = arg_eq<0>(100);
  auto cond1 = arg_ne<1>(200);
  auto cond = cond0 || cond1;

  EXPECT_EQ(cond(100, 201), true);	// T || T
  EXPECT_EQ(cond(100, 200), true);	// T || F
  EXPECT_EQ(cond(101, 201), true);	// F || T
  EXPECT_EQ(cond(101, 200), false);	// F || F
}

TEST_F(ConditionTests, NotOperator)
{
  auto cond0 = arg_eq<0>(100);
  auto cond = !cond0;

  EXPECT_EQ(cond(100), false);		// ! T
  EXPECT_EQ(cond(101), true);		// ! F
}
