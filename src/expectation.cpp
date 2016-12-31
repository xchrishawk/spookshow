/**
 * @file	expectation.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2016/12/30
 */

/* -- Includes -- */

#include <sstream>
#include <string>

#include <spookshow/spookshow.hpp>

/* -- Namespaces -- */

using namespace spookshow;

/* -- Procedures -- */

expectation::expectation(const std::string& name, int required_count)
  : m_name(name),
    m_required_count(required_count),
    m_order(expectation_order::current_order()),
    m_count(0)
{
  if (m_order)
    m_order->enqueue_expectation(this);
}

expectation::~expectation()
{
  if (is_fulfilled())
    return;

  std::ostringstream message;
  message << "Unfulfilled expectation! [" << name()
          << "] Expected " << m_required_count << " call" << (m_required_count == 1 ? "" : "s")
          << ", received " << m_count << " call" << (m_count == 1 ? "" : "s") << ".";
  internal::handle_failure(message.str());
}

void expectation::fulfill()
{
  if (m_order && m_count == 0)
  {
    if (m_order->is_expectation_next(this))
      m_order->dequeue_expectation();
    else
    {
      std::ostringstream message;
      message << "Expectation fulfilled out of order! [" << name() << "]";
      internal::handle_failure(message.str());
    }
  }
  ++m_count;
}
