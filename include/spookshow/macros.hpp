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

#define SPOOKSHOW_MAKE_MOCK_METHOD_2_IMPL_(ret, meth, constness, t0, t1)			\
  virtual ret meth(t0 arg0, t1 arg1) constness override						\
  {												\
    SPOOKSHOW_METHOD_OBJ_(meth).set_name(__PRETTY_FUNCTION__);					\
    return SPOOKSHOW_METHOD_OBJ_(meth).invoke(arg0, arg1);					\
  }												\
  spookshow::method<ret(t0, t1)> SPOOKSHOW_METHOD_OBJ_(meth) { #meth }

#define SPOOKSHOW_MAKE_MOCK_METHOD_3_IMPL_(ret, meth, constness, t0, t1, t2) 			\
  virtual ret meth(t0 arg0, t1 arg1, t2 arg2) constness override				\
  {												\
    SPOOKSHOW_METHOD_OBJ_(meth).set_name(__PRETTY_FUNCTION__);					\
    return SPOOKSHOW_METHOD_OBJ_(meth).invoke(arg0, arg1, arg2);				\
  }												\
  spookshow::method<ret(t0, t1, t2)> SPOOKSHOW_METHOD_OBJ_(meth) { #meth }

#define SPOOKSHOW_MAKE_MOCK_METHOD_4_IMPL_(ret, meth, constness, t0, t1, t2, t3) 		\
  virtual ret meth(t0 arg0, t1 arg1, t2 arg2, t3 arg3) constness override			\
  {												\
    SPOOKSHOW_METHOD_OBJ_(meth).set_name(__PRETTY_FUNCTION__);					\
    return SPOOKSHOW_METHOD_OBJ_(meth).invoke(arg0, arg1, arg2, arg3);				\
  }												\
  spookshow::method<ret(t0, t1, t2, t3)> SPOOKSHOW_METHOD_OBJ_(meth) { #meth }

#define SPOOKSHOW_MAKE_MOCK_METHOD_5_IMPL_(ret, meth, constness, t0, t1, t2, t3, t4) 		\
  virtual ret meth(t0 arg0, t1 arg1, t2 arg2, t3 arg3, t4 arg4) constness override 		\
  {												\
    SPOOKSHOW_METHOD_OBJ_(meth).set_name(__PRETTY_FUNCTION__);					\
    return SPOOKSHOW_METHOD_OBJ_(meth).invoke(arg0, arg1, arg2, arg3, arg4);			\
  }												\
  spookshow::method<ret(t0, t1, t2, t3, t4)> SPOOKSHOW_METHOD_OBJ_(meth) { #meth }


/* -- Public Macros -- */

#define SPOOKSHOW_MOCK_METHOD(obj, meth)							\
  (&((obj).SPOOKSHOW_METHOD_OBJ_(meth)))

#define SPOOKSHOW_MAKE_MOCK_METHOD_0(ret, meth)							\
  SPOOKSHOW_MAKE_MOCK_METHOD_0_IMPL_(ret, meth, )

#define SPOOKSHOW_MAKE_MOCK_METHOD_1(ret, meth, t0)						\
  SPOOKSHOW_MAKE_MOCK_METHOD_1_IMPL_(ret, meth, , t0)

#define SPOOKSHOW_MAKE_MOCK_METHOD_2(ret, meth, t0, t1)						\
  SPOOKSHOW_MAKE_MOCK_METHOD_2_IMPL_(ret, meth, , t0, t1)

#define SPOOKSHOW_MAKE_MOCK_METHOD_3(ret, meth, t0, t1, t2)					\
  SPOOKSHOW_MAKE_MOCK_METHOD_3_IMPL_(ret, meth, , t0, t1, t2)

#define SPOOKSHOW_MAKE_MOCK_METHOD_4(ret, meth, t0, t1, t2, t3)					\
  SPOOKSHOW_MAKE_MOCK_METHOD_4_IMPL_(ret, meth, , t0, t1, t2, t3)

#define SPOOKSHOW_MAKE_MOCK_METHOD_5(ret, meth, t0, t1, t2, t3, t4)				\
  SPOOKSHOW_MAKE_MOCK_METHOD_5_IMPL_(ret, meth, , t0, t1, t2, t3, t4)


#define SPOOKSHOW_MAKE_CONST_MOCK_METHOD_0(ret, meth)						\
  SPOOKSHOW_MAKE_MOCK_METHOD_0_IMPL_(ret, meth, const)

#define SPOOKSHOW_MAKE_CONST_MOCK_METHOD_1(ret, meth, t0)					\
  SPOOKSHOW_MAKE_MOCK_METHOD_1_IMPL_(ret, meth, const, t0)

#define SPOOKSHOW_MAKE_CONST_MOCK_METHOD_2(ret, meth, t0, t1)					\
  SPOOKSHOW_MAKE_MOCK_METHOD_2_IMPL_(ret, meth, const, t0, t1)

#define SPOOKSHOW_MAKE_CONST_MOCK_METHOD_3(ret, meth, t0, t1, t2)				\
  SPOOKSHOW_MAKE_MOCK_METHOD_3_IMPL_(ret, meth, const, t0, t1, t2)

#define SPOOKSHOW_MAKE_CONST_MOCK_METHOD_4(ret, meth, t0, t1, t2, t3)				\
  SPOOKSHOW_MAKE_MOCK_METHOD_4_IMPL_(ret, meth, const, t0, t1, t2, t3)

#define SPOOKSHOW_MAKE_CONST_MOCK_METHOD_5(ret, meth, t0, t1, t2, t3, t4)			\
  SPOOKSHOW_MAKE_MOCK_METHOD_5_IMPL_(ret, meth, const, t0, t1, t2, t3, t4)
