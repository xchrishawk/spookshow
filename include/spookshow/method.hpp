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

#include <spookshow/spookshow.hpp>

/* -- Types -- */

namespace spookshow
{

  namespace internal
  {

    /**
     * Token for enqueuing a no-op functor.
     */
    class noops_token { };

    /**
     * Token for returning a value.
     */
    template <typename T>
    class returns_token
    {
    public:

      explicit returns_token(T value)
	: m_value(value)
      { }

      /** Returns the value. */
      T value() const
      {
	return m_value;
      }

    private:

      T m_value;

    };

  }

  template <typename TRet, typename... TArgs>
  class method;

  /**
   * Object providing functionality for mocking a method.
   */
  template <typename TRet, typename... TArgs>
  class method<TRet(TArgs...)>
  {
  private:

    static const int INFINITE = -1;

    typedef std::function<TRet(TArgs...)> functor;

    /**
     * Class representing an entry in the functor queue.
     */
    class functor_entry
    {
    public:

    private:

      friend class method<TRet(TArgs...)>;

      int m_count;
      functor m_functor;

      functor_entry(int count, const functor& functor)
	: m_count(count),
	  m_functor(functor)
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

	if (entry->m_count != INFINITE)
	{
	  entry->m_count = std::max(entry->m_count - 1, 0);
	  if (entry->m_count == 0)
	    m_functor_queue.pop();
	}

	return entry->m_functor(args...);
      }
      else
      {
	std::ostringstream message;
	message << "Unexpected mock method call! [" << m_name << "].";
	internal::handle_failure(message.str());
	return TRet();
      }
    }

    /**
     * Enqueues a no-op which may be performed once.
     */
    template <typename... _>
    std::shared_ptr<functor_entry> once(const internal::noops_token&)
    {
      return once([] (TArgs...) -> void { });
    }

    /**
     * Enqueues a value which may be returned once.
     */
    template <typename T>
    std::shared_ptr<functor_entry> once(const internal::returns_token<T>& token)
    {
      return once([token] (TArgs...) -> T {
	  return token.value();
	});
    }

    /**
     * Enqueues a functor which may be performed once.
     */
    std::shared_ptr<functor_entry> once(const functor& functor)
    {
      return enqueue_functor(1, functor);
    }

    /**
     * Enqueues a no-op which may be performed a finite number of times.
     */
    template <typename... _>
    std::shared_ptr<functor_entry> repeat(int count, const internal::noops_token&)
    {
      return repeat(count, [] (TArgs...) -> void { });
    }

    /**
     * Enqueues a value which may be returned a finite number of times.
     */
    template <typename T>
    std::shared_ptr<functor_entry> repeat(int count, const internal::returns_token<T>& token)
    {
      return repeat(count, [token] (TArgs...) -> T {
	  return token.value();
	});
    }

    /**
     * Enqueues a functor which may be performed a finite number of times.
     */
    std::shared_ptr<functor_entry> repeat(int count, const functor& functor)
    {
      return enqueue_functor(count, functor);
    }

    /**
     * Enqueues a no-op which may be performed an infinite number of times.
     */
    template <typename... _>
    std::shared_ptr<functor_entry> always(const internal::noops_token&)
    {
      return always([] (TArgs...) -> void { });
    }

    /**
     * Enqueues a value which may be returned an infinite number of times.
     */
    template <typename T>
    std::shared_ptr<functor_entry> always(const internal::returns_token<T>& token)
    {
      return always([token] (TArgs...) -> T {
	  return token.value();
	});
    }

    /**
     * Enqueues a functor which may be performed an infinite number of times.
     */
    std::shared_ptr<functor_entry> always(const functor& functor)
    {
      return enqueue_functor(INFINITE, functor);
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
    std::shared_ptr<functor_entry> enqueue_functor(int count, const functor& functor)
    {
      if (count < 1 && count != INFINITE)
	internal::handle_error("Specified functor count was invalid!");

      auto entry = std::shared_ptr<functor_entry>(new functor_entry(count, functor));
      m_functor_queue.push(entry);
      return entry;
    }

    mutable std::string m_name;
    mutable std::queue<std::shared_ptr<functor_entry>> m_functor_queue;

  };

}

namespace spookshow
{

  /**
   * Creates an `internal::noops` token.
   */
  inline internal::noops_token noops()
  {
    return internal::noops_token();
  }

  /**
   * Creates an `internal::returns` token.
   */
  template <typename T>
  inline internal::returns_token<T> returns(T value)
  {
    return internal::returns_token<T>(value);
  }

}
