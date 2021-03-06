/**
 * @file	spookshow.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2016/12/30
 */

#pragma once
#define SPOOKSHOW_HPP_INCLUDED

/* -- Includes -- */

#include <functional>

/* -- Types -- */

namespace spookshow
{

  /** Typedef for functions which report a test failure. */
  typedef std::function<void(const std::string&)> fail_handler;

}

/* -- Procedure Prototypes -- */

namespace spookshow
{

  /**
   * Sets the failure handler used by the Spookshow library.
   */
  void set_fail_handler(fail_handler handler);

  namespace internal
  {

    /**
     * Reports a test failure to the failure handler.
     */
    void handle_failure(const std::string& message);

    /**
     * Handles an unrecoverable test logic error. This method does not return.
     */
    [[noreturn]] void handle_error(const std::string& message);

  }

}

/* -- Library Includes -- */

#include <spookshow/condition.hpp>
#include <spookshow/expectation.hpp>
#include <spookshow/expectation_order.hpp>
#include <spookshow/macros.hpp>
#include <spookshow/method.hpp>
