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
    m_count(0)
{
}

expectation::~expectation()
{
  if (m_count >= m_required_count)
    return;

  std::ostringstream message;
  message << "Unfulfilled expectation [" << (m_name.empty() ? "No Name" : m_name)
          << "] Expected " << m_required_count << " call" << (m_required_count == 1 ? "" : "s")
          << ", received " << m_count << " call" << (m_count == 1 ? "" : "s") << ".";
  internal::handle_failure(message.str());
}

void expectation::fulfill()
{
  ++m_count;
}
