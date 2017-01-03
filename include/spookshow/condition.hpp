/**
 * @file	condition.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/05
 */

#pragma once

/* -- Includes -- */

#include <spookshow/spookshow.hpp>

/* -- Types -- */

namespace spookshow
{

  namespace internal
  {

    /**
     * Functor class encapsulating a condition on a method call.
     */
    template <typename TLambda>
    class condition_functor final
    {
    public:

      /**
       * Creates a new `condition_functor` wrapping the specified condition lambda.
       */
      explicit condition_functor(const TLambda& lambda)
        : m_lambda(lambda)
      { }

      /**
       * Executes the condition lambda on the specified arguments, and returns `true` if it
       * succeeds.
       */
      template <typename... TArgs>
      bool operator ()(const TArgs&... args) const
      {
        return m_lambda(args...);
      }

    private:
      TLambda m_lambda;
    };

  }

}

/* -- Procedures -- */

namespace spookshow
{

  /**
   * Creates a condition requiring that an argument be equal to a specific value.
   */
  template <int Index, typename TValue>
  inline auto arg_eq(const TValue& value)
  {
    auto lambda = [value] (auto... args) -> bool {
      return (std::get<Index>(std::tuple<decltype(args)...>(args...)) == value);
    };
    return spookshow::internal::condition_functor<decltype(lambda)>(lambda);
  }

  /**
   * Creates a condition requiring that an argument be not equal to a specific value.
   */
  template <int Index, typename TValue>
  inline auto arg_ne(const TValue& value)
  {
    auto lambda = [value] (auto... args) -> bool {
      return (std::get<Index>(std::tuple<decltype(args)...>(args...)) != value);
    };
    return spookshow::internal::condition_functor<decltype(lambda)>(lambda);
  }

  /**
   * Creates a condition from a logical AND of two other conditions.
   */
  template <typename TFirstLambda, typename TSecondLambda>
  inline auto operator &&(const spookshow::internal::condition_functor<TFirstLambda>& first,
                          const spookshow::internal::condition_functor<TSecondLambda>& second)
  {
    auto lambda = [first, second] (auto... args) -> bool {
      return (first(args...) && second(args...));
    };
    return spookshow::internal::condition_functor<decltype(lambda)>(lambda);
  }

  /**
   * Creates a condition from a logical OR of two other conditions.
   */
  template <typename TFirstLambda, typename TSecondLambda>
  inline auto operator ||(const spookshow::internal::condition_functor<TFirstLambda>& first,
                          const spookshow::internal::condition_functor<TSecondLambda>& second)
  {
    auto lambda = [first, second] (auto... args) -> bool {
      return (first(args...) || second(args...));
    };
    return spookshow::internal::condition_functor<decltype(lambda)>(lambda);
  }

  /**
   * Creates a condition from a logical NOT of another condition.
   */
  template <typename TLambda>
  inline auto operator !(const spookshow::internal::condition_functor<TLambda>& cond)
  {
    auto lambda = [cond] (auto... args) -> bool {
      return !cond(args...);
    };
    return spookshow::internal::condition_functor<decltype(lambda)>(lambda);
  }

}
