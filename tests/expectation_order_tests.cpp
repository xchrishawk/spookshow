/**
 * @file	expectation_order_tests.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2016/12/31
 */

/* -- Includes -- */

#include "test_base.hpp"

/* -- Namespaces -- */

using namespace spookshow;
using namespace testing;

/* -- Test Cases -- */

/**
 * Unit test for the `spookshow::expectation_order` class.
 */
class ExpectationOrderTests : public ::spookshow::tests::TestBase
{
};

TEST_F(ExpectationOrderTests, DoesNotFailIfExpectationsCalledInOrder)
{
  expectation_order order;

  expectation exp1;
  expectation exp2;
  expectation exp3;

  exp1.fulfill();
  exp2.fulfill();
  exp3.fulfill();

  EXPECT_NOT_FAILED();
}

TEST_F(ExpectationOrderTests, FailsIfExpectationsCalledOutOfOrder)
{
  // for scope
  expectation_order order;

  expectation exp1;
  expectation exp2;
  expectation exp3;

  exp1.fulfill();
  EXPECT_NOT_FAILED();

  exp3.fulfill();
  EXPECT_FAILED();
}

TEST_F(ExpectationOrderTests, ExpectationOrderStackOperatesCorrectly)
{
  expectation_order outer_order;

  expectation outer_exp1;
  expectation outer_exp2;
  expectation outer_exp3;

  outer_exp1.fulfill();
  EXPECT_NOT_FAILED();

  // inner scope
  {
    expectation_order inner_order;

    expectation inner_exp1;
    expectation inner_exp2;
    expectation inner_exp3;

    inner_exp1.fulfill();
    EXPECT_NOT_FAILED();

    inner_exp3.fulfill();
    EXPECT_FAILED();

    inner_exp2.fulfill();
  }

  reset_failed();

  outer_exp3.fulfill();
  EXPECT_FAILED();
}
