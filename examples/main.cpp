/**
 * @file	main.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2016/12/30
 */

/* -- Includes -- */

#include <iostream>
#include <spookshow/spookshow.hpp>

#include "object.hpp"
#include "object_mock.hpp"

/* -- Namespaces -- */

using namespace spookshow;

/* -- Procedure Prototypes -- */

namespace
{
  void example_fail_handler(const std::string& message);
  void expectation_demo();
  void expectation_order_demo();
  void expectation_order_stack_demo();
  void mock_lambda_demo();
  void noops_demo();
  void returns_demo();
}

/* -- Procedures -- */

int main(int argc, char** argv)
{
  set_fail_handler(example_fail_handler);
  expectation_demo();
  expectation_order_demo();
  expectation_order_stack_demo();
  mock_lambda_demo();
  noops_demo();
  returns_demo();
  return 0;
}

namespace
{

  /**
   * Failure handler for the Spookshow library.
   *
   * In this method, you would normally forward the error message to whichever unit testing library
   * you're using as a test driver. For this simple demo, we just print the error message to
   * standard error.
   */
  void example_fail_handler(const std::string& message)
  {
    std::cerr << "<<TEST FAIL>> " << message << std::endl;
  }

  /**
   * Simple expectations demo.
   */
  void expectation_demo()
  {
    std::cout << "-- Expectation Demo --" << std::endl << std::endl;

    std::cout << "Entering first scope..." << std::endl;
    {
      expectation not_fulfilled("This expectation will fail");

      expectation fulfilled("This expectation will not fail");
      fulfilled.fulfill();
    }
    std::cout << "Exited first scope." << std::endl << std::endl;

    std::cout << "Entering second scope..." << std::endl;
    {
      expectation not_fulfilled_multiple("Requires 3+, but only called twice.", 3);
      not_fulfilled_multiple.fulfill();
      not_fulfilled_multiple.fulfill();

      expectation fulfilled_multiple("This expectation will not fail.", 2);
      fulfilled_multiple.fulfill();
      fulfilled_multiple.fulfill();
    }
    std::cout << "Exited second scope." << std::endl << std::endl;
  }

  /**
   * Simple expectation order demo.
   */
  void expectation_order_demo()
  {
    std::cout << "-- Expectation Order Demo --" << std::endl << std::endl;

    std::cout << "Entering first scope..." << std::endl;
    {
      expectation_order order;

      expectation first("First expectation");
      expectation second("Second expectation");
      expectation third("Third expectation");

      first.fulfill();
      second.fulfill();
      third.fulfill();
    }
    std::cout << "Exited first scope." << std::endl << std::endl;

    std::cout << "Entering second scope..." << std::endl;
    {
      expectation_order order;

      expectation first("First expectation");
      expectation second("Second expectation");
      expectation third("Third expectation");

      first.fulfill();
      third.fulfill();
      second.fulfill();
    }
    std::cout << "Exited second scope." << std::endl << std::endl;
  }

  /**
   * Demo of expectation order stack.
   */
  void expectation_order_stack_demo()
  {
    std::cout << "-- Expectation Order Stack Demo --" << std::endl << std::endl;

    std::cout << "Entering outer scope..." << std::endl;
    {
      expectation_order outer_order;

      expectation outer_first("First outer expectation");
      expectation outer_second("Second outer expectation");
      expectation outer_third("Third outer expectation");

      outer_first.fulfill();

      std::cout << "Entering inner scope..." << std::endl;
      {
	expectation_order inner_order;

	expectation inner_first("First inner expectation");
	expectation inner_second("Second inner expectation");

	inner_second.fulfill();
	inner_first.fulfill();

	outer_second.fulfill();
      }
      std::cout << "Exited inner scope." << std::endl;

      expectation outer_fourth("Fourth outer expectation");

      outer_third.fulfill();
      outer_fourth.fulfill();
    }
    std::cout << "Exited outer scope." << std::endl << std::endl;
  }

  void mock_lambda_demo()
  {
    std::cout << "-- Mock Lambda Demo --" << std::endl << std::endl;

    examples::object_mock mock;

    std::cout << "Enqueuing functor with once(), then calling twice..." << std::endl;
    SPOOKSHOW_MOCK_METHOD(mock, void_no_args)->once([] {
	std::cout << "(From Functor) This can only be performed once." << std::endl;
      });

    mock.void_no_args();
    mock.void_no_args();

    std::cout << std::endl << "Enqueuing functor with repeat(2), then calling 3 times..." << std::endl;
    SPOOKSHOW_MOCK_METHOD(mock, void_no_args)->repeat(2, [] {
	std::cout << "(From Functor) This can be called 3 times." << std::endl;
      });

    mock.void_no_args();
    mock.void_no_args();
    mock.void_no_args();

    std::cout << std::endl << "Enqueuing functor with always(), then calling 3 times..." << std::endl;
    SPOOKSHOW_MOCK_METHOD(mock, void_no_args)->always([] {
	std::cout << "(From Functor) This can be called an infinite number of times." << std::endl;
      });

    mock.void_no_args();
    mock.void_no_args();
    mock.void_no_args();

    std::cout << std::endl << "Enqueuing functor calculating factorial, then calling with 10..." << std::endl;
    SPOOKSHOW_MOCK_METHOD(mock, int_one_arg)->always([] (int value) {
	std::cout << "(From Functor) Hello from the factorial lambda!" << std::endl;
	int fact = 1;
	while (value > 1)
	  fact *= value--;
	return fact;
      });

    std::cout << "Returned: " << mock.int_one_arg(10) << std::endl;

    std::cout << std::endl;
  }

  void noops_demo()
  {
    std::cout << "-- Noops Demo --" << std::endl << std::endl;

    examples::object_mock mock;

    std::cout << "Enqueuing noops with once(), then calling twice..." << std::endl;
    SPOOKSHOW_MOCK_METHOD(mock, void_no_args)->once(noops());

    mock.void_no_args();
    mock.void_no_args();

    std::cout << "Enqueuing noops with repeat(2), then calling 3 times..." << std::endl;
    SPOOKSHOW_MOCK_METHOD(mock, void_one_arg)->repeat(2, noops());

    mock.void_one_arg(15);
    mock.void_one_arg(20);
    mock.void_one_arg(25);

    std::cout << "Enqueuing noops with always(), then calling 3 times..." << std::endl;
    SPOOKSHOW_MOCK_METHOD(mock, void_one_arg)->always(noops());

    mock.void_one_arg(15);
    mock.void_one_arg(20);
    mock.void_one_arg(25);

    // this would fail to compile, becase the functor type must return a value
    // SPOOKSHOW_MOCK_METHOD(mock, void_no_args)->once(noops());

    std::cout << std::endl;
  }

  void returns_demo()
  {
    std::cout << "-- Returns Demo --" << std::endl << std::endl;

    examples::object_mock mock;

    std::cout << "Enqueuing returns(1) with once(), then calling twice..." << std::endl;
    SPOOKSHOW_MOCK_METHOD(mock, int_no_args)->once(returns(1));

    std::cout << "Returned: " << mock.int_no_args() << std::endl;
    std::cout << "Returned: " << mock.int_no_args() << std::endl;

    std::cout << "Enqueuing returns(2) with repeat(2), then calling 3 times..." << std::endl;
    SPOOKSHOW_MOCK_METHOD(mock, int_one_arg)->repeat(2, returns(2));

    std::cout << "Returned: " << mock.int_one_arg(5) << std::endl;
    std::cout << "Returned: " << mock.int_one_arg(10) << std::endl;
    std::cout << "Returned: " << mock.int_one_arg(15) << std::endl;

    std::cout << "Enqueuing returns(3) with always(), then calling 3 times..." << std::endl;
    SPOOKSHOW_MOCK_METHOD(mock, int_one_arg)->always(returns(3));

    std::cout << "Returned: " << mock.int_one_arg(5) << std::endl;
    std::cout << "Returned: " << mock.int_one_arg(10) << std::endl;
    std::cout << "Returned: " << mock.int_one_arg(15) << std::endl;

    // this would fail to compile, because the functor type must return a value
    // SPOOKSHOW_MOCK_METHOD(mock, void_no_args)->once(returns());

    std::cout << std::endl;
  }
}
