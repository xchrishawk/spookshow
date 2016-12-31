/**
 * @file	spookshow.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2016/12/30
 */

#pragma once
#define SPOOKSHOW_HPP_INCLUDED

/* -- Includes -- */

#include <functional>

#include <spookshow/expectation.hpp>

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

  }

}
