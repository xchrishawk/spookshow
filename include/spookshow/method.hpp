/**
 * @file	method.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2016/12/30
 */

#pragma once

/* -- Includes -- */

#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <spookshow/spookshow.hpp>

/* -- Types -- */

namespace spookshow
{

  namespace internal
  {

    /**
     * Token for enqueuing a no-op functor.
     */
    class noops_token final { };

    /**
     * Token for returning a value.
     */
    template <typename TValue>
    class returns_token final
    {
    public:

      explicit returns_token(const TValue& value)
	: m_value(value)
      { }

      /** Returns the value. */
      TValue value() const
      {
	return m_value;
      }

    private:
      TValue m_value;
    };

    // required to use "function" syntax in class template
    template <typename TRet, typename... TArgs>
    class method;

    /**
     * Object providing functionality for mocking a method.
     */
    template <typename TRet, typename... TArgs>
    class method<TRet(TArgs...)> final
    {
    private:

      static const int INFINITE = -1;

      using functor = std::function<TRet(TArgs...)>;
      using condition = std::function<bool(TArgs...)>;

      /**
       * Class representing an entry in the functor queue.
       */
      class functor_entry
      {
      public:

        /**
         * Adds a condition which must be true before this method may be called.
         */
        functor_entry* requires(const condition& condition)
        {
          m_conditions.push_back(condition);
          return this;
        }

        /**
         * Adds an expectation to be fulfilled by a successful invocation of this method.
         */
        functor_entry* fulfills(expectation& expectation)
        {
          m_expectations.push_back(&expectation);
          return this;
        }

      private:

        friend class method<TRet(TArgs...)>;

        const functor m_functor;
        int m_count;
        std::vector<condition> m_conditions;
        std::vector<expectation*> m_expectations;

        functor_entry(const functor& functor, int count)
          : m_functor(functor),
            m_count(count),
            m_conditions(),
            m_expectations()
        { }

      };

    public:

      /**
       * Creates a new mock method object.
       *
       * @param name
       * The name of the method.
       */
      explicit method(const std::string& name)
        : m_name(name)
      { }

      /**
       * Updates the name of the method with a more accurate string.
       */
      void set_name(const std::string& name) const
      {
        m_name = name;
      }

      /**
       * Invokes the mock method with the specified arguments.
       */
      TRet invoke(TArgs... args) const
      {
        if (!m_functor_queue.empty())
        {
          const auto& entry = m_functor_queue.front();

          // we have to cache the functor in case we delete the entry from the queue
          functor functor_copy = entry->m_functor;

          // check conditions on this call
          for (const auto& condition : entry->m_conditions)
            if (!condition(args...))
            {
              std::ostringstream message;
              message << "Mock method call with unexpected arguments! [" << m_name << "].";
              spookshow::internal::handle_failure(message.str());
              return TRet();
            }

          // fulfill all expectations for this call
          for (const auto exp : entry->m_expectations)
            exp->fulfill();

          // clear the entry from the queue if we're out of available calls
          if (entry->m_count != INFINITE)
          {
            entry->m_count = std::max(entry->m_count - 1, 0);
            if (entry->m_count == 0)
              m_functor_queue.pop();
          }

          return functor_copy(args...);
        }
        else
        {
          std::ostringstream message;
          message << "Unexpected mock method call! [" << m_name << "].";
          spookshow::internal::handle_failure(message.str());
          return TRet();
        }
      }

      /**
       * Removes the functor at the front of the queue.
       *
       * This can be used (for example) to clear a functor which was enqueued with `repeats()` or
       * `always()`, but which is no longer needed.
       */
      void skip() const
      {
        m_functor_queue.pop();
      }

      /**
       * Clears all functors from the queue.
       *
       * This essentially resets the mock method to its initial state.
       */
      void reset() const
      {
        std::queue<std::shared_ptr<functor_entry>> empty;
        std::swap(m_functor_queue, empty);
      }

      /**
       * Enqueues a no-op which may be performed once.
       */
      std::shared_ptr<functor_entry> once(const spookshow::internal::noops_token&) const
      {
        return once([] (TArgs...) -> void { });
      }

      /**
       * Enqueues a value which may be returned once.
       */
      template <typename TValue>
        std::shared_ptr<functor_entry> once(const spookshow::internal::returns_token<TValue>& token) const
      {
        return once([token] (TArgs...) -> TValue {
            return token.value();
          });
      }

      /**
       * Enqueues a functor which may be performed once.
       */
      std::shared_ptr<functor_entry> once(const functor& functor) const
      {
        return enqueue_functor(functor, 1);
      }

      /**
       * Enqueues a no-op which may be performed a finite number of times.
       */
      std::shared_ptr<functor_entry> repeats(int count, const spookshow::internal::noops_token&) const
      {
        return repeats(count, [] (TArgs...) -> void { });
      }

      /**
       * Enqueues a value which may be returned a finite number of times.
       */
      template <typename TValue>
        std::shared_ptr<functor_entry> repeats(int count, const spookshow::internal::returns_token<TValue>& token) const
      {
        return repeats(count, [token] (TArgs...) -> TValue {
            return token.value();
          });
      }

      /**
       * Enqueues a functor which may be performed a finite number of times.
       */
      std::shared_ptr<functor_entry> repeats(int count, const functor& functor) const
      {
        return enqueue_functor(functor, count);
      }

      /**
       * Enqueues a no-op which may be performed an infinite number of times.
       */
      std::shared_ptr<functor_entry> always(const spookshow::internal::noops_token&) const
      {
        return always([] (TArgs...) -> void { });
      }

      /**
       * Enqueues a value which may be returned an infinite number of times.
       */
      template <typename TValue>
        std::shared_ptr<functor_entry> always(const spookshow::internal::returns_token<TValue>& token) const
      {
        return always([token] (TArgs...) -> TValue {
            return token.value();
          });
      }

      /**
       * Enqueues a functor which may be performed an infinite number of times.
       */
      std::shared_ptr<functor_entry> always(const functor& functor) const
      {
        return enqueue_functor(functor, INFINITE);
      }

    private:

      /**
       * Enqueues a new functor.
       *
       * @param functor
       * The functor to enqueue.
       *
       * @param count
       * The number of times this functor may be executed.
       */
      std::shared_ptr<functor_entry> enqueue_functor(const functor& functor, int count) const
      {
        if (count < 1 && count != INFINITE)
          spookshow::internal::handle_error("Specified functor count was invalid!");

        auto entry = std::shared_ptr<functor_entry>(new functor_entry(functor, count));
        m_functor_queue.push(entry);
        return entry;
      }

      mutable std::string m_name;
      mutable std::queue<std::shared_ptr<functor_entry>> m_functor_queue;

    };

  }

}

/* -- Procedures -- */

namespace spookshow
{

  /**
   * Creates a token indicating that a method call should do nothing.
   */
  inline spookshow::internal::noops_token noops()
  {
    return spookshow::internal::noops_token();
  }

  /**
   * Creates a token indicating that a method call should return a specific value.
   */
  template <typename TValue>
  inline spookshow::internal::returns_token<TValue> returns(const TValue& value)
  {
      return spookshow::internal::returns_token<TValue>(value);
  }

  /**
   * Creates a token indicating that a method call should return a specific value.
   *
   * @note
   * This overload is required for array types, particularly string literals for strings returning `const char*`.
   */
  template <typename TValue>
  inline spookshow::internal::returns_token<TValue*> returns(TValue* value)
  {
    return spookshow::internal::returns_token<TValue*>(value);
  }

}
