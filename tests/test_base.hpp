/**
 * @file	test_base.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/05
 */

#pragma once

/* -- Includes -- */

#include <string>
#include <gtest/gtest.h>
#include <spookshow/spookshow.hpp>

/* -- Types -- */

namespace spookshow
{
  namespace tests
  {

    /**
     * Base class for Spookshow unit tests.
     */
    class TestBase : public ::testing::Test
    {
    protected:

      bool m_failed { false };
      std::string m_fail_message;

      virtual void SetUp() override
      {
        spookshow::set_fail_handler([this] (const std::string& fail_message) {
            this->m_failed = true;
            this->m_fail_message = fail_message;
          });
      };

      virtual void TearDown() override
      {
        spookshow::set_fail_handler(nullptr);
      }

      void reset_failed()
      {
        m_failed = false;
        m_fail_message = std::string();
      }

    };

  }
}

/* -- Macros -- */

// these are macros to preserve line number information in gtest failure report
#define EXPECT_NOT_FAILED()								        \
  EXPECT_FALSE(m_failed) << "Unexpected failure occurred!"
#define EXPECT_FAILED()										\
  EXPECT_TRUE(m_failed) << "Expected failure did not occur!"
