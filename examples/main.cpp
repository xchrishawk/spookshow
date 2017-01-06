/**
 * @file	main.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2016/12/30
 */

/* -- Includes -- */

#include <functional>
#include <iostream>
#include <map>

#include <spookshow/spookshow.hpp>

#include "object.hpp"
#include "object_mock.hpp"

/* -- Namespaces -- */

using namespace spookshow;

/* -- Procedure Prototypes -- */

namespace
{
  void example_fail_handler(const std::string& message);

  void run_all();
  void print_help();

  void expectation_demo();
  void expectation_order_demo();
  void expectation_order_stack_demo();
  void mock_lambda_demo();
  void noops_demo();
  void returns_demo();
  void const_demo();
  void skip_reset_demo();
}

/* -- Constants -- */

namespace
{
  static const std::map<std::string, std::function<void()>> DEMOS =
  {
    { "expectation", 			expectation_demo },
    { "expectation_order", 		expectation_order_demo },
    { "expectation_order_stack",	expectation_order_stack_demo },
    { "mock_lambda",			mock_lambda_demo },
    { "noops",				noops_demo },
    { "returns",			returns_demo },
    { "const",				const_demo },
    { "skip_reset",			skip_reset_demo },
  };
}

/* -- Procedures -- */

int main(int argc, char** argv)
{
  set_fail_handler(example_fail_handler);

  if (argc <= 1)
  {
    print_help();
    return 0;
  }

  for (int arg = 1; arg < argc; arg++)
  {
    const std::string example(argv[arg]);
    if (example == "--all")
      run_all();
    else if (example == "--help")
      print_help();
    else
    {
      auto it = DEMOS.find(example);
      if (it != DEMOS.end())
        it->second();
      else
        std::cout << "Unknown example: " << example << std::endl;
    }
  }

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
   * Runs all examples.
   */
  void run_all()
  {
    for (const auto& item : DEMOS)
      item.second();
  }

  /**
   * Prints the help screen.
   */
  void print_help()
  {
    std::cout << "Usage: examples [example1] [example2]..." << std::endl << std::endl;
    std::cout << "List of examples:" << std::endl;
    for (const auto& item : DEMOS)
      std::cout << "- " <<  item.first << std::endl;
    std::cout << std::endl;
    std::cout << "examples --all: Runs all examples" << std::endl;
    std::cout << "examples --help: Displays this help info" << std::endl;
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
    SPOOKSHOW(mock, void_no_args)->once([] {
	std::cout << "(From Functor) This can only be performed once." << std::endl;
      });

    mock.void_no_args();
    mock.void_no_args();

    std::cout << std::endl << "Enqueuing functor with repeats(2), then calling 3 times..." << std::endl;
    SPOOKSHOW(mock, void_no_args)->repeats(2, [] {
	std::cout << "(From Functor) This can be called 2 times." << std::endl;
      });

    mock.void_no_args();
    mock.void_no_args();
    mock.void_no_args();

    std::cout << std::endl << "Enqueuing functor with always(), then calling 3 times..." << std::endl;
    SPOOKSHOW(mock, void_no_args)->always([] {
	std::cout << "(From Functor) This can be called an infinite number of times." << std::endl;
      });

    mock.void_no_args();
    mock.void_no_args();
    mock.void_no_args();

    std::cout << std::endl << "Enqueuing functor calculating factorial, then calling with 10..." << std::endl;
    SPOOKSHOW(mock, int_one_arg)->always([] (int value) {
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
    SPOOKSHOW(mock, void_no_args)->once(noops());

    mock.void_no_args();
    mock.void_no_args();

    std::cout << "Enqueuing noops with repeats(2), then calling 3 times..." << std::endl;
    SPOOKSHOW(mock, void_one_arg)->repeats(2, noops());

    mock.void_one_arg(15);
    mock.void_one_arg(20);
    mock.void_one_arg(25);

    std::cout << "Enqueuing noops with always(), then calling 3 times..." << std::endl;
    SPOOKSHOW(mock, void_one_arg)->always(noops());

    mock.void_one_arg(15);
    mock.void_one_arg(20);
    mock.void_one_arg(25);

    // this would fail to compile, becase the functor type must return a value
    // SPOOKSHOW(mock, void_no_args)->once(noops());

    std::cout << std::endl;
  }

  void returns_demo()
  {
    std::cout << "-- Returns Demo --" << std::endl << std::endl;

    examples::object_mock mock;

    std::cout << "Enqueuing returns(1) with once(), then calling twice..." << std::endl;
    SPOOKSHOW(mock, int_no_args)->once(returns(1));

    std::cout << "Returned: " << mock.int_no_args() << std::endl;
    std::cout << "Returned: " << mock.int_no_args() << std::endl;

    std::cout << "Enqueuing returns(2) with repeats(2), then calling 3 times..." << std::endl;
    SPOOKSHOW(mock, int_one_arg)->repeats(2, returns(2));

    std::cout << "Returned: " << mock.int_one_arg(5) << std::endl;
    std::cout << "Returned: " << mock.int_one_arg(10) << std::endl;
    std::cout << "Returned: " << mock.int_one_arg(15) << std::endl;

    std::cout << "Enqueuing returns(3) with always(), then calling 3 times..." << std::endl;
    SPOOKSHOW(mock, int_one_arg)->always(returns(3));

    std::cout << "Returned: " << mock.int_one_arg(5) << std::endl;
    std::cout << "Returned: " << mock.int_one_arg(10) << std::endl;
    std::cout << "Returned: " << mock.int_one_arg(15) << std::endl;

    // this would fail to compile, because the functor type must return a value
    // SPOOKSHOW(mock, void_no_args)->once(returns());

    std::cout << std::endl;
  }

  void const_demo()
  {
    std::cout << "-- Const Demo --" << std::endl << std::endl;

    const examples::object_mock mock;

    std::cout << "Mock object is const, but we can still mock methods with a different macro:" << std::endl;
    SPOOKSHOW(mock, const_void_no_args)->once([] {
        std::cout << "(From Functor) Hello from a mocked const method!" << std::endl;
      });

    mock.const_void_no_args();

    std::cout << std::endl;
  }

  void skip_reset_demo()
  {
    std::cout << "-- Skip/Reset Demo --" << std::endl << std::endl;

    examples::object_mock mock;

    std::cout << "Queuing a functor with always(), and calling 3 times..." << std::endl;
    SPOOKSHOW(mock, void_no_args)->always([] {
        std::cout << "(From First Functor) This will repeat forever..." << std::endl;
      });

    mock.void_no_args();
    mock.void_no_args();
    mock.void_no_args();

    std::cout << "Queing another functor with always(), and calling once..." << std::endl;
    SPOOKSHOW(mock, void_no_args)->always([] {
        std::cout << "(From Second Functor) This will repeat forever, but only after the first one is cleared" << std::endl;
      });

    mock.void_no_args();

    std::cout << "Calling skip() to pop the first functor from the queue, and calling once..." << std::endl;
    SPOOKSHOW(mock, void_no_args)->skip();

    mock.void_no_args();

    std::cout << "Queuing several more functors with once()..." << std::endl;
    SPOOKSHOW(mock, void_no_args)->once(noops());
    SPOOKSHOW(mock, void_no_args)->once(noops());
    SPOOKSHOW(mock, void_no_args)->once(noops());

    std::cout << "Calling reset() to reset the queue, and calling once..." << std::endl;
    SPOOKSHOW(mock, void_no_args)->reset();

    mock.void_no_args();

    std::cout << std::endl;
  }

}
