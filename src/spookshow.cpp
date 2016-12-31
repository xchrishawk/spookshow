/**
 * @file	spookshow.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2016/12/30
 */

/* -- Includes -- */

#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include <spookshow/spookshow.hpp>

/* -- Namespaces -- */

using namespace spookshow;

/* -- Variables -- */

namespace
{
  fail_handler user_fail_handler;
}

/* -- Procedures -- */

void spookshow::set_fail_handler(fail_handler handler)
{
  user_fail_handler = handler;
}

void spookshow::internal::handle_failure(const std::string& message)
{
  if (user_fail_handler)
    user_fail_handler(message);
  else
    handle_error("Spookshow fail handler was not set!");
}

[[noreturn]] void spookshow::internal::handle_error(const std::string& message)
{
  std::cerr << message << std::endl;
  std::abort();
}
