/**
 * @file	main.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2016/12/30
 */

/* -- Includes -- */

#include <iostream>
#include <spookshow/spookshow.hpp>

/* -- Namespaces -- */

using namespace spookshow;

/* -- Procedure Prototypes -- */

namespace
{
  void example_fail_handler(const std::string& message);
  void expectation_demo();
  void expectation_order_demo();
  void expectation_order_stack_demo();
}

/* -- Procedures -- */

int main(int argc, char** argv)
{
  set_fail_handler(example_fail_handler);
  expectation_demo();
  expectation_order_demo();
  expectation_order_stack_demo();
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
    std::cerr << message << std::endl;
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
    std::cout << "Exited outer scope." << std::endl;
  }

}
