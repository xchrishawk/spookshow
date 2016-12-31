/**
 * @file	expectation_order_tests.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2016/12/31
 */

/* -- Includes -- */

#include <string>

#include <gtest/gtest.h>
#include <spookshow/spookshow.hpp>

/* -- Namespaces -- */

using namespace spookshow;
using namespace testing;

/* -- Test Cases -- */

/**
 * Unit test for the `spookshow::expectation_order` class.
 */
class ExpectationOrderTest : public Test
{
protected:

  bool m_failed { false };
  std::string m_fail_message;

  virtual void SetUp() override
  {
    set_fail_handler([this] (const std::string& fail_message) {
        this->m_failed = true;
        this->m_fail_message = fail_message;
      });
  };

  virtual void TearDown() override
  {
    set_fail_handler(nullptr);
  }

  void reset_failed()
  {
    m_failed = false;
    m_fail_message = std::string();
  }

};

// these are macros to preserve line number information in gtest failure report
#define expect_not_failed()								        \
  EXPECT_FALSE(m_failed) << "Expectation order failed when it should not have failed!"
#define expect_failed()										\
  EXPECT_TRUE(m_failed) << "Expectation order did not fail when it should have failed!"

TEST_F(ExpectationOrderTest, DoesNotFailIfExpectationsCalledInOrder)
{
  expectation_order order;

  expectation exp1;
  expectation exp2;
  expectation exp3;

  exp1.fulfill();
  exp2.fulfill();
  exp3.fulfill();

  expect_not_failed();
}

TEST_F(ExpectationOrderTest, FailsIfExpectationsCalledOutOfOrder)
{
  // for scope
  expectation_order order;

  expectation exp1;
  expectation exp2;
  expectation exp3;

  exp1.fulfill();
  expect_not_failed();

  exp3.fulfill();
  expect_failed();
}

TEST_F(ExpectationOrderTest, ExpectationOrderStackOperatesCorrectly)
{
  expectation_order outer_order;

  expectation outer_exp1;
  expectation outer_exp2;
  expectation outer_exp3;

  outer_exp1.fulfill();
  expect_not_failed();

  // inner scope
  {
    expectation_order inner_order;

    expectation inner_exp1;
    expectation inner_exp2;
    expectation inner_exp3;

    inner_exp1.fulfill();
    expect_not_failed();

    inner_exp3.fulfill();
    expect_failed();

    inner_exp2.fulfill();
  }

  reset_failed();

  outer_exp3.fulfill();
  expect_failed();
}
