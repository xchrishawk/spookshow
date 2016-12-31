/**
 * @file	expectation_order.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2016/12/30
 */

/* -- Includes -- */

#include <cstdlib>
#include <spookshow/spookshow.hpp>

/* -- Namespaces -- */

using namespace spookshow;

/* -- Variables -- */

std::stack<expectation_order*> expectation_order::s_orders;

/* -- Procedures -- */

expectation_order::expectation_order()
  : m_expectations()
{
  s_orders.push(this);
}

expectation_order::~expectation_order()
{
  if (s_orders.top() != this)
    internal::handle_error("Expectation order stack was corrupted!");
  s_orders.pop();
}

expectation_order* expectation_order::current_order()
{
  return (s_orders.empty() ? nullptr : s_orders.top());
}

void expectation_order::enqueue_expectation(const expectation* exp)
{
  m_expectations.push(exp);
}

void expectation_order::dequeue_expectation()
{
  m_expectations.pop();
}

bool expectation_order::is_expectation_next(const expectation* exp) const
{
  return (m_expectations.empty() ? false : m_expectations.front() == exp);
}
