/**
 * @file	object.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2016/12/30
 */

#pragma once

/* -- Types -- */

namespace spookshow
{
  namespace examples
  {

    /**
     * This is an example object which will be mocked in the example app.
     */
    class object
    {
    public:

      /**
       * Function taking no arguments and returning no value.
       */
      virtual void void_no_args()
      {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
      }

      /**
       * Function taking one argument and returning no value.
       */
      virtual void void_one_arg(int num)
      {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
      }

      /**
       * Function taking two arguments and returning no value.
       */
      virtual void void_two_args(int num, const std::string& str)
      {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
      }

      /**
       * Function taking no arguments and returning an `int`.
       */
      virtual int int_no_args()
      {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	return 100;
      }

      /**
       * Function taking one argument and returning an `int`.
       */
      virtual int int_one_arg(int num)
      {
	std::cout << __PRETTY_FUNCTION__ << std::endl;
	return num * 100;
      }

      /**
       * Const function taking no arguments and returning no value.
       */
      virtual void const_void_no_args() const
      {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
      }

    };

  }
}
