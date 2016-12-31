/**
 * @file	main.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2016/12/30
 */

/* -- Includes -- */

#include <iostream>
#include <spookshow/spookshow.hpp>

/* -- Procedure Prototypes -- */

namespace
{
  void fail_handler(const std::string& message);
  void expectation_demo();
}

/* -- Procedures -- */

int main(int argc, char** argv)
{
  spookshow::set_fail_handler(fail_handler);
  expectation_demo();
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
  void fail_handler(const std::string& message)
  {
    std::cerr << "Spookshow failure! " << message << std::endl;
  }

  /**
   * Simple expectations demo.
   */
  void expectation_demo()
  {
    std::cout << "-- Expectation Demo --" << std::endl << std::endl;

    std::cout << "Entering first scope..." << std::endl;
    {
      spookshow::expectation not_fulfilled("This expectation will fail");

      spookshow::expectation fulfilled("This expectation will not fail");
      fulfilled.fulfill();
    }
    std::cout << "Exited first scope." << std::endl << std::endl;

    std::cout << "Entering second scope..." << std::endl;
    {
      spookshow::expectation not_fulfilled_multiple("Requires 3+, but only called twice.", 3);
      not_fulfilled_multiple.fulfill();
      not_fulfilled_multiple.fulfill();

      spookshow::expectation fulfilled_multiple("This expectation will not fail.", 2);
      fulfilled_multiple.fulfill();
      fulfilled_multiple.fulfill();
    }
    std::cout << "Exited second scope." << std::endl << std::endl;
  }

}
