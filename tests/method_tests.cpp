/**
 * @file	method_tests.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2016/12/31
 */

/* -- Includes -- */

#include "test_base.hpp"

/* -- Namespaces -- */

using namespace spookshow;
using namespace testing;

/* -- Object Definition -- */

namespace
{

  /**
   * Sample object which will be mocked.
   */
  class object
  {
  public:
    virtual void void_no_args() const { }
    virtual void void_one_arg(int value) { }
    virtual void void_two_args(int value1, int value2) { }
    virtual void void_pointer_arg(int* value) { }
    virtual int int_no_args() { return 0; }
    virtual int int_one_arg(int value) { return 0; }
    virtual int int_two_args(int value1, int value2) { return 0; }
    virtual char* returns_pointer() { return nullptr; }
    virtual const char* returns_const_pointer() { return nullptr; }
    virtual std::string returns_string() { return ""; }
  };

/**
 * A mock object for the `object` class.
 */
  class mock : public object
  {
  public:
    SPOOKSHOW_MOCK_CONST_METHOD_0(void, void_no_args);
    SPOOKSHOW_MOCK_METHOD_1(void, void_one_arg, int);
    SPOOKSHOW_MOCK_METHOD_2(void, void_two_args, int, int);
    SPOOKSHOW_MOCK_METHOD_1(void, void_pointer_arg, int*);
    SPOOKSHOW_MOCK_METHOD_0(int, int_no_args);
    SPOOKSHOW_MOCK_METHOD_1(int, int_one_arg, int);
    SPOOKSHOW_MOCK_METHOD_2(int, int_two_args, int, int);
    SPOOKSHOW_MOCK_METHOD_0(char*, returns_pointer);
    SPOOKSHOW_MOCK_METHOD_0(const char*, returns_const_pointer);
    SPOOKSHOW_MOCK_METHOD_0(std::string, returns_string);
  };

}

/* -- Test Cases -- */

/**
 * Unit test for the `spookshow::method` class.
 */
class MethodTests : public ::spookshow::tests::TestBase
{
protected:
  mock m_mock;
};

TEST_F(MethodTests, DoesNotFailIfExpectedMethodIsCalled)
{
  SPOOKSHOW(m_mock, void_no_args).once(noops());
  m_mock.void_no_args();
  EXPECT_NOT_FAILED();
}

TEST_F(MethodTests, FailsIfUnexpectedMethodIsCalled)
{
  m_mock.void_no_args();
  EXPECT_FAILED();
}

TEST_F(MethodTests, OnceQueuesFunctorOnce)
{
  SPOOKSHOW(m_mock, void_no_args).once(noops());

  m_mock.void_no_args();
  EXPECT_NOT_FAILED();

  m_mock.void_no_args();
  EXPECT_FAILED();
}

TEST_F(MethodTests, RepeatsQueuesFunctorMultipleTimes)
{
  static const int COUNT = 5;
  SPOOKSHOW(m_mock, void_no_args).repeats(COUNT, noops());

  for (int idx = 0; idx < COUNT; idx++)
    m_mock.void_no_args();
  EXPECT_NOT_FAILED();

  m_mock.void_no_args();
  EXPECT_FAILED();
}

TEST_F(MethodTests, AlwaysQueuesFunctorInfiniteTimes)
{
  SPOOKSHOW(m_mock, void_no_args).always(noops());

  for (int idx = 0; idx < 1000; idx++)
    m_mock.void_no_args();
  EXPECT_NOT_FAILED();
}

TEST_F(MethodTests, FunctorsAreExecutedInOrderTheyAreQueued)
{
  static const int EXPECTED_TIMES_ONCE_CALLED = 1;
  static const int EXPECTED_TIMES_REPEATS_CALLED = 3;
  static const int EXPECTED_TIMES_ALWAYS_CALLED = 1000;
  static const int TOTAL_CALLS =
    EXPECTED_TIMES_ONCE_CALLED +
    EXPECTED_TIMES_REPEATS_CALLED +
    EXPECTED_TIMES_ALWAYS_CALLED;

  int times_once_called = 0;
  int times_repeats_called = 0;
  int times_always_called = 0;

  SPOOKSHOW(m_mock, void_no_args).once([&] {
      ++times_once_called;
    });
  SPOOKSHOW(m_mock, void_no_args).repeats(EXPECTED_TIMES_REPEATS_CALLED, [&] {
      ++times_repeats_called;
    });
  SPOOKSHOW(m_mock, void_no_args).always([&] {
      ++times_always_called;
    });

  for (int idx = 0; idx < TOTAL_CALLS; idx++)
    m_mock.void_no_args();

  EXPECT_EQ(times_once_called, EXPECTED_TIMES_ONCE_CALLED);
  EXPECT_EQ(times_repeats_called, EXPECTED_TIMES_REPEATS_CALLED);
  EXPECT_EQ(times_always_called, EXPECTED_TIMES_ALWAYS_CALLED);
}

TEST_F(MethodTests, SkipRemovesNextFunctorInQueue)
{
  bool once_called = false;

  SPOOKSHOW(m_mock, void_no_args).always(noops());
  SPOOKSHOW(m_mock, void_no_args).once([&] {
      once_called = true;
    });

  for (int idx = 0; idx < 1000; idx++)
    m_mock.void_no_args();
  EXPECT_NOT_FAILED();
  EXPECT_FALSE(once_called);

  SPOOKSHOW(m_mock, void_no_args).skip();

  m_mock.void_no_args();
  EXPECT_NOT_FAILED();
  EXPECT_TRUE(once_called);

  m_mock.void_no_args();
  EXPECT_FAILED();
}

TEST_F(MethodTests, ResetRemovesAllFunctorsInQueue)
{
  SPOOKSHOW(m_mock, void_no_args).always(noops());
  SPOOKSHOW(m_mock, void_no_args).always(noops());
  SPOOKSHOW(m_mock, void_no_args).always(noops());

  m_mock.void_no_args();
  EXPECT_NOT_FAILED();

  SPOOKSHOW(m_mock, void_no_args).reset();

  m_mock.void_no_args();
  EXPECT_FAILED();
}


TEST_F(MethodTests, NoopsDoesNothing)
{
  SPOOKSHOW(m_mock, void_one_arg).once(noops());
  m_mock.void_one_arg(15);
  EXPECT_NOT_FAILED();
}

TEST_F(MethodTests, ReturnsReturnsValueType)
{
  static const int EXPECTED_RETURN = 2000;
  SPOOKSHOW(m_mock, int_one_arg).once(returns(EXPECTED_RETURN));
  EXPECT_EQ(m_mock.int_one_arg(-55), EXPECTED_RETURN);
  EXPECT_NOT_FAILED();
}

TEST_F(MethodTests, ReturnsReturnsPointerType)
{
  char EXPECTED_RETURN[] = "test";
  SPOOKSHOW(m_mock, returns_pointer).once(returns(EXPECTED_RETURN));
  EXPECT_EQ(m_mock.returns_pointer(), EXPECTED_RETURN);
  EXPECT_NOT_FAILED();
}

TEST_F(MethodTests, ReturnsReturnsConstPointerType)
{
  static const char* EXPECTED_RETURN = "const string";
  SPOOKSHOW(m_mock, returns_const_pointer).once(returns(EXPECTED_RETURN));
  EXPECT_EQ(m_mock.returns_const_pointer(), EXPECTED_RETURN);
  EXPECT_NOT_FAILED();
}

TEST_F(MethodTests, ReturnsReturnsString)
{
  static const char* EXPECTED_RETURN = "should be converted to std::string implicitly";
  SPOOKSHOW(m_mock, returns_string).once(returns(EXPECTED_RETURN));
  EXPECT_EQ(m_mock.returns_string(), EXPECTED_RETURN);
  EXPECT_NOT_FAILED();
}

TEST_F(MethodTests, DoesNotFailWithSuccessfulCondition)
{
  static const int EXPECTED_ARG = -100;
  SPOOKSHOW(m_mock, void_one_arg).once(noops()).requires([] (int arg) {
      return (arg == EXPECTED_ARG);
    });
  m_mock.void_one_arg(EXPECTED_ARG);
  EXPECT_NOT_FAILED();
}

TEST_F(MethodTests, FailsWithUnsuccessfulCondition)
{
  static const int EXPECTED_ARG = -4000;
  SPOOKSHOW(m_mock, void_one_arg).once(noops()).requires([] (int arg) {
      return (arg == EXPECTED_ARG);
    });
  m_mock.void_one_arg(EXPECTED_ARG + 1);
  EXPECT_FAILED();
}

TEST_F(MethodTests, ExpectationFulfilledOnCallWithNoConditions)
{
  expectation exp;
  SPOOKSHOW(m_mock, void_no_args).once(noops()).fulfills(exp);
  m_mock.void_no_args();
  EXPECT_TRUE(exp.is_fulfilled());
  EXPECT_NOT_FAILED();
}

TEST_F(MethodTests, ExpectationFulfilledOnCallWithSuccessfulCondition)
{
  static const int EXPECTED_ARG = 100;
  expectation exp;
  SPOOKSHOW(m_mock, void_one_arg).once(noops()).fulfills(exp).requires([] (int arg) {
      return (arg == EXPECTED_ARG);
    });
  m_mock.void_one_arg(EXPECTED_ARG);
  EXPECT_TRUE(exp.is_fulfilled());
  EXPECT_NOT_FAILED();
}

TEST_F(MethodTests, ExpectationNotFulfilledOnCallWithUnsuccessfulCondition)
{
  static const int EXPECTED_ARG = 200;
  expectation exp;
  SPOOKSHOW(m_mock, void_one_arg).once(noops()).fulfills(exp).requires([] (int arg) {
      return (arg == EXPECTED_ARG);
    });
  m_mock.void_one_arg(EXPECTED_ARG + 1);
  EXPECT_FALSE(exp.is_fulfilled());
  EXPECT_FAILED();
}

TEST_F(MethodTests, MultipleExpectationsCanBeFulfilledWithOneCall)
{
  expectation exp1;
  expectation exp2;
  SPOOKSHOW(m_mock, void_no_args).once(noops()).fulfills(exp1).fulfills(exp2);
  m_mock.void_no_args();
  EXPECT_TRUE(exp1.is_fulfilled());
  EXPECT_TRUE(exp2.is_fulfilled());
  EXPECT_NOT_FAILED();
}

TEST_F(MethodTests, ExpectationOrderDoesNotFailWhenMethodsCalledInExpectedOrder)
{
  expectation_order order;
  expectation exp1;
  expectation exp2;
  SPOOKSHOW(m_mock, void_no_args).once(noops()).fulfills(exp1);
  SPOOKSHOW(m_mock, void_one_arg).once(noops()).fulfills(exp2);
  m_mock.void_no_args();
  m_mock.void_one_arg(0);
  EXPECT_NOT_FAILED();
}

TEST_F(MethodTests, ExpectationOrderFailsWhenMethodsCalledOutOfExpectedOrder)
{
  expectation_order order;
  expectation exp1;
  expectation exp2;
  SPOOKSHOW(m_mock, void_no_args).once(noops()).fulfills(exp1);
  SPOOKSHOW(m_mock, void_one_arg).once(noops()).fulfills(exp2);
  m_mock.void_one_arg(0);
  EXPECT_FAILED();
}

TEST_F(MethodTests, DoesNotFailWithSuccessfulConditionFunctorObject)
{
  static const int EXPECTED_ARG_1 = 100;
  static const int EXPECTED_ARG_2 = 200;
  SPOOKSHOW(m_mock, void_two_args)
    .once(noops())
    .requires(arg_eq<0>(EXPECTED_ARG_1) && arg_ne<1>(EXPECTED_ARG_2));
  m_mock.void_two_args(EXPECTED_ARG_1, EXPECTED_ARG_2 + 1);
  EXPECT_NOT_FAILED();
}

TEST_F(MethodTests, FailsWithUnsuccessfulConditionFunctorObject)
{
  static const int EXPECTED_ARG_1 = 100;
  static const int EXPECTED_ARG_2 = 200;
  SPOOKSHOW(m_mock, void_two_args)
    .once(noops())
    .requires(arg_eq<0>(EXPECTED_ARG_1) && arg_ne<1>(EXPECTED_ARG_2));
  m_mock.void_two_args(EXPECTED_ARG_1, EXPECTED_ARG_2);
  EXPECT_FAILED();
}
