/**
 * @file	expectation_tests.cpp
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
 * Unit test for the `spookshow::expectation` class.
 */
class ExpectationTest : public Test
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
  }

  virtual void TearDown() override
  {
    set_fail_handler(nullptr);
  }

};

// these are macros to preserve line number information in gtest failure report
#define expect_not_failed()								        \
  EXPECT_FALSE(m_failed) << "Expectation failed when it should not have failed!"
#define expect_failed()										\
  EXPECT_TRUE(m_failed) << "Expectation did not fail when it should have failed!"

TEST_F(ExpectationTest, DoesNotFailWhenLeavingScopeIfFulfilled)
{
  // for scope
  {
    expectation exp("This should not fail");
    exp.fulfill();
  }
  expect_not_failed();
}

TEST_F(ExpectationTest, FailsWhenLeavingScopeIfNotFulfilled)
{
  // for scope
  {
    expectation exp("This should fail");
  }
  expect_failed();
}

TEST_F(ExpectationTest, DoesNotFailWhenLeavingScopeIfFulfilledWithMultipleCalls)
{
  // for scope
  {
    expectation exp("This should not fail", 3);
    exp.fulfill();
    exp.fulfill();
    exp.fulfill();
  }
  expect_not_failed();
}

TEST_F(ExpectationTest, FailsWhenLeavingScopeIfNotFulfilledWithMultipleCalls)
{
  // for scope
  {
    expectation exp("This should fail", 3);
    exp.fulfill();
    exp.fulfill();
  }
  expect_failed();
}

TEST_F(ExpectationTest, ExpectationCanBeCalledMoreThanRequiredNumberOfTimes)
{
  // for scope
  {
    expectation exp("This should not fail", 2);
    exp.fulfill();
    exp.fulfill();
    exp.fulfill();
  }
  expect_not_failed();
}

TEST_F(ExpectationTest, ExpectationFailureMessageReportsExpectationName)
{
  static const std::string EXPECTED_NAME = "The Name";
  // for scope
  {
    expectation exp(EXPECTED_NAME);
  }
  expect_failed();
  EXPECT_NE(m_fail_message.find(EXPECTED_NAME), std::string::npos);
}
