/**
 * @file	macros.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2016/12/30
 */

// much ugliness follows...

/* -- Includes -- */

#include <spookshow/spookshow.hpp>

/* -- Implementation Macros -- */

// these are for implementation, do not use these in user code

#define SPOOKSHOW_METHOD_OBJECT_(meth)								\
  SPOOKSHOW_MOCK_METHOD_ ## meth ## _

#define SPOOKSHOW_MOCK_METHOD_0_IMPL_(ret, meth, cvqual)					\
  virtual ret meth(void) cvqual override							\
  {												\
    SPOOKSHOW_METHOD_OBJECT_(meth).set_name(__PRETTY_FUNCTION__);				\
    return SPOOKSHOW_METHOD_OBJECT_(meth).invoke();						\
  }												\
  spookshow::internal::method<ret()> SPOOKSHOW_METHOD_OBJECT_(meth) { #meth }

#define SPOOKSHOW_MOCK_METHOD_1_IMPL_(ret, meth, cvqual, t0)					\
  virtual ret meth(t0 arg0) cvqual override							\
  {												\
    SPOOKSHOW_METHOD_OBJECT_(meth).set_name(__PRETTY_FUNCTION__);				\
    return SPOOKSHOW_METHOD_OBJECT_(meth).invoke(arg0);						\
  }												\
  spookshow::internal::method<ret(t0)> SPOOKSHOW_METHOD_OBJECT_(meth) { #meth }

#define SPOOKSHOW_MOCK_METHOD_2_IMPL_(ret, meth, cvqual, t0, t1)				\
  virtual ret meth(t0 arg0, t1 arg1) cvqual override						\
  {												\
    SPOOKSHOW_METHOD_OBJECT_(meth).set_name(__PRETTY_FUNCTION__);				\
    return SPOOKSHOW_METHOD_OBJECT_(meth).invoke(arg0, arg1);					\
  }												\
  spookshow::internal::method<ret(t0, t1)> SPOOKSHOW_METHOD_OBJECT_(meth) { #meth }

#define SPOOKSHOW_MOCK_METHOD_3_IMPL_(ret, meth, cvqual, t0, t1, t2) 				\
  virtual ret meth(t0 arg0, t1 arg1, t2 arg2) cvqual override					\
  {												\
    SPOOKSHOW_METHOD_OBJECT_(meth).set_name(__PRETTY_FUNCTION__);				\
    return SPOOKSHOW_METHOD_OBJECT_(meth).invoke(arg0, arg1, arg2);				\
  }												\
  spookshow::internal::method<ret(t0, t1, t2)> SPOOKSHOW_METHOD_OBJECT_(meth) { #meth }

#define SPOOKSHOW_MOCK_METHOD_4_IMPL_(ret, meth, cvqual, t0, t1, t2, t3) 			\
  virtual ret meth(t0 arg0, t1 arg1, t2 arg2, t3 arg3) cvqual override				\
  {												\
    SPOOKSHOW_METHOD_OBJECT_(meth).set_name(__PRETTY_FUNCTION__);				\
    return SPOOKSHOW_METHOD_OBJECT_(meth).invoke(arg0, arg1, arg2, arg3);			\
  }												\
  spookshow::internal::method<ret(t0, t1, t2, t3)> SPOOKSHOW_METHOD_OBJECT_(meth) { #meth }

#define SPOOKSHOW_MOCK_METHOD_5_IMPL_(ret, meth, cvqual, t0, t1, t2, t3, t4) 			\
  virtual ret meth(t0 arg0, t1 arg1, t2 arg2, t3 arg3, t4 arg4) cvqual override 		\
  {												\
    SPOOKSHOW_METHOD_OBJECT_(meth).set_name(__PRETTY_FUNCTION__);				\
    return SPOOKSHOW_METHOD_OBJECT_(meth).invoke(arg0, arg1, arg2, arg3, arg4);			\
  }												\
  spookshow::internal::method<ret(t0, t1, t2, t3, t4)> SPOOKSHOW_METHOD_OBJECT_(meth) { #meth }


/* -- Public Macros -- */

/**
 * Returns the Spookshow mock method for the specified object and method name.
 */
#define SPOOKSHOW(obj, meth)									\
  ((obj).SPOOKSHOW_METHOD_OBJECT_(meth))

/**
 * Creates a mock for a non-`const` method with no arguments.
 */
#define SPOOKSHOW_MOCK_METHOD_0(ret, meth)							\
  SPOOKSHOW_MOCK_METHOD_0_IMPL_(ret, meth, )

/**
 * Creates a mock for a non-`const` method with one argument.
 */
#define SPOOKSHOW_MOCK_METHOD_1(ret, meth, t0)							\
  SPOOKSHOW_MOCK_METHOD_1_IMPL_(ret, meth, , t0)

/**
 * Creates a mock for a non-`const` method with two arguments.
 */
#define SPOOKSHOW_MOCK_METHOD_2(ret, meth, t0, t1)						\
  SPOOKSHOW_MOCK_METHOD_2_IMPL_(ret, meth, , t0, t1)

/**
 * Creates a mock for a non-`const` method with three arguments.
 */
#define SPOOKSHOW_MOCK_METHOD_3(ret, meth, t0, t1, t2)						\
  SPOOKSHOW_MOCK_METHOD_3_IMPL_(ret, meth, , t0, t1, t2)

/**
 * Creates a mock for a non-`const` method with four arguments.
 */
#define SPOOKSHOW_MOCK_METHOD_4(ret, meth, t0, t1, t2, t3)					\
  SPOOKSHOW_MOCK_METHOD_4_IMPL_(ret, meth, , t0, t1, t2, t3)

/**
 * Creates a mock for a non-`const` method with five arguments.
 */
#define SPOOKSHOW_MOCK_METHOD_5(ret, meth, t0, t1, t2, t3, t4)					\
  SPOOKSHOW_MOCK_METHOD_5_IMPL_(ret, meth, , t0, t1, t2, t3, t4)

/**
 * Creates a mock for a `const` method with no arguments.
 */
#define SPOOKSHOW_MOCK_CONST_METHOD_0(ret, meth)						\
  SPOOKSHOW_MOCK_METHOD_0_IMPL_(ret, meth, const)

/**
 * Creates a mock for a `const` method with one argument.
 */
#define SPOOKSHOW_MOCK_CONST_METHOD_1(ret, meth, t0)						\
  SPOOKSHOW_MOCK_METHOD_1_IMPL_(ret, meth, const, t0)

/**
 * Creates a mock for a `const` method with two arguments.
 */
#define SPOOKSHOW_MOCK_CONST_METHOD_2(ret, meth, t0, t1)					\
  SPOOKSHOW_MOCK_METHOD_2_IMPL_(ret, meth, const, t0, t1)

/**
 * Creates a mock for a `const` method with three arguments.
 */
#define SPOOKSHOW_MOCK_CONST_METHOD_3(ret, meth, t0, t1, t2)					\
  SPOOKSHOW_MOCK_METHOD_3_IMPL_(ret, meth, const, t0, t1, t2)

/**
 * Creates a mock for a `const` method with four arguments.
 */
#define SPOOKSHOW_MOCK_CONST_METHOD_4(ret, meth, t0, t1, t2, t3)				\
  SPOOKSHOW_MOCK_METHOD_4_IMPL_(ret, meth, const, t0, t1, t2, t3)

/**
 * Creates a mock for a `const` method with five arguments.
 */
#define SPOOKSHOW_MOCK_CONST_METHOD_5(ret, meth, t0, t1, t2, t3, t4)				\
  SPOOKSHOW_MOCK_METHOD_5_IMPL_(ret, meth, const, t0, t1, t2, t3, t4)
