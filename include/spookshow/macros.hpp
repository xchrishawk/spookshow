/**
 * @file	macros.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2016/12/30
 */

// much ugliness follows...

/* -- Includes -- */

#include <spookshow/spookshow.hpp>

/* -- Implementation Macros -- */

#define SPOOKSHOW_METHOD_OBJ_(meth)								\
  __SPOOKSHOW_MOCK_ ## meth ## _

#define SPOOKSHOW_MAKE_MOCK_METHOD_0_IMPL_(ret, meth, constness)				\
  virtual ret meth(void) constness override							\
  {												\
    SPOOKSHOW_METHOD_OBJ_(meth).set_name(__PRETTY_FUNCTION__);					\
    return SPOOKSHOW_METHOD_OBJ_(meth).invoke();						\
  }												\
  spookshow::method<ret()> SPOOKSHOW_METHOD_OBJ_(meth) { #meth }

#define SPOOKSHOW_MAKE_MOCK_METHOD_1_IMPL_(ret, meth, constness, t0)				\
  virtual ret meth(t0 arg0) constness override							\
  {												\
    SPOOKSHOW_METHOD_OBJ_(meth).set_name(__PRETTY_FUNCTION__);					\
    return SPOOKSHOW_METHOD_OBJ_(meth).invoke(arg0);						\
  }												\
  spookshow::method<ret(t0)> SPOOKSHOW_METHOD_OBJ_(meth) { #meth }

/* -- Public Macros -- */

#define SPOOKSHOW_MOCK_METHOD(obj, meth)							\
  (&((obj).SPOOKSHOW_METHOD_OBJ_(meth)))

#define SPOOKSHOW_MAKE_MOCK_METHOD_0(ret, meth)							\
  SPOOKSHOW_MAKE_MOCK_METHOD_0_IMPL_(ret, meth, )

#define SPOOKSHOW_MAKE_MOCK_METHOD_1(ret, meth, t0)						\
  SPOOKSHOW_MAKE_MOCK_METHOD_1_IMPL_(ret, meth, , t0)

#define SPOOKSHOW_MAKE_CONST_MOCK_METHOD_0(ret, meth)						\
  SPOOKSHOW_MAKE_MOCK_METHOD_0_IMPL_(ret, meth, const)

#define SPOOKSHOW_MAKE_CONST_MOCK_METHOD_1(ret, meth, t0)					\
  SPOOKSHOW_MAKE_MOCK_METHOD_1_IMPL_(ret, meth, const, t0)
