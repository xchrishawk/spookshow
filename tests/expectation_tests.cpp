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
class ExpectationTests : public Test
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
#define EXPECT_NOT_FAILED()								        \
  EXPECT_FALSE(m_failed) << "Unexpected failure occurred!"
#define EXPECT_FAILED()										\
  EXPECT_TRUE(m_failed) << "Expected failure did not occur!"

TEST_F(ExpectationTests, DoesNotFailWhenLeavingScopeIfFulfilled)
{
  // for scope
  {
    expectation exp("This should not fail");
    exp.fulfill();
  }
  EXPECT_NOT_FAILED();
}

TEST_F(ExpectationTests, FailsWhenLeavingScopeIfNotFulfilled)
{
  // for scope
  {
    expectation exp("This should fail");
  }
  EXPECT_FAILED();
}

TEST_F(ExpectationTests, DoesNotFailWhenLeavingScopeIfFulfilledWithMultipleCalls)
{
  // for scope
  {
    expectation exp("This should not fail", 3);
    exp.fulfill();
    exp.fulfill();
    exp.fulfill();
  }
  EXPECT_NOT_FAILED();
}

TEST_F(ExpectationTests, FailsWhenLeavingScopeIfNotFulfilledWithMultipleCalls)
{
  // for scope
  {
    expectation exp("This should fail", 3);
    exp.fulfill();
    exp.fulfill();
  }
  EXPECT_FAILED();
}

TEST_F(ExpectationTests, ExpectationCanBeCalledMoreThanRequiredNumberOfTimes)
{
  // for scope
  {
    expectation exp("This should not fail", 2);
    exp.fulfill();
    exp.fulfill();
    exp.fulfill();
  }
  EXPECT_NOT_FAILED();
}

TEST_F(ExpectationTests, ExpectationFailureMessageReportsExpectationName)
{
  static const std::string EXPECTED_NAME = "The Name";
  // for scope
  {
    expectation exp(EXPECTED_NAME);
  }
  EXPECT_FAILED();
  EXPECT_NE(m_fail_message.find(EXPECTED_NAME), std::string::npos);
}
