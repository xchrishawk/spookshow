/**
 * @file	expectation.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2016/12/30
 */

#pragma once

/* -- Includes -- */

#include <string>
#include <spookshow/spookshow.hpp>

/* -- Types -- */

namespace spookshow
{

  /**
   * Class representing an expectation which must be fulfilled.
   */
  class expectation final
  {
  private:

    static const int MINIMUM_REQUIRED_COUNT = 1;

  public:

    /**
     * Creates a new expectation with no name and a required count of one.
     */
    expectation()
      : expectation(std::string(), MINIMUM_REQUIRED_COUNT)
    { }

    /**
     * Creates a new expectation with a required count of one.
     *
     * @param name
     * The name of the expectation.
     */
    expectation(const std::string& name)
      : expectation(name, MINIMUM_REQUIRED_COUNT)
    { }

    /**
     * Creates a new expectation with no name.
     *
     * @param required_count
     * The number of times the requirement must be fulfilled.
     */
    expectation(int required_count)
      : expectation(std::string(), required_count)
    { }

    /**
     * Creates a new expectation.
     *
     * @param name
     * The name of the expectation.
     *
     * @param required_count
     * The number of times the requirement must be fulfilled.
     */
    expectation(const std::string& name, int required_count);

    ~expectation();

  private:

    expectation(const expectation&) = delete;
    expectation& operator =(const expectation&) = delete;

  public:

    /**
     * Fulfills this expectation once.
     */
    void fulfill();

  private:

    const std::string m_name;
    const int m_required_count;
    int m_count;

  };

}
