/**
 * @file	expectation_order.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2016/12/30
 */

#pragma once

/* -- Includes -- */

#include <queue>
#include <stack>

#include <spookshow/spookshow.hpp>

/* -- Types -- */

namespace spookshow
{

  class expectation;

  /**
   * Class imposing an order on expectation fulfillment.
   *
   * This class has no public API. It works with `spookshow::expectation` to enforce an expected
   * order in which expectations must be fulfilled.
   */
  class expectation_order
  {
  public:

    expectation_order();
    ~expectation_order();

  private:

    expectation_order(const expectation_order&) = delete;
    expectation_order& operator =(const expectation_order&) = delete;

    friend class expectation;

    static std::stack<expectation_order*> s_orders;
    std::queue<const expectation*> m_expectations;

    static expectation_order* current_order();
    void enqueue_expectation(const expectation* exp);
    void dequeue_expectation();
    bool is_expectation_next(const expectation* exp) const;

  };

}
