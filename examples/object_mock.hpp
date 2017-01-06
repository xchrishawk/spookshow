/**
 * @file	object_mock.hpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2016/12/30
 */

/* -- Includes -- */

#include <spookshow/spookshow.hpp>

/* -- Types -- */

namespace spookshow
{
  namespace examples
  {

    /**
     * Mocked version of the `spookshow::examples::object` class.
     */
    class object_mock : public object
    {
    public:

      // These macros are defined in <spookshow/macros.h>.
      // - They override the corresponding method in the superclass.
      // - They create mock method objects which may be accessed with the SPOOKSHOW() macro.
      SPOOKSHOW_MOCK_METHOD_0(void, void_no_args);
      SPOOKSHOW_MOCK_METHOD_1(void, void_one_arg, int);
      SPOOKSHOW_MOCK_METHOD_0(int, int_no_args);
      SPOOKSHOW_MOCK_METHOD_1(int, int_one_arg, int);
      SPOOKSHOW_MOCK_CONST_METHOD_0(void, const_void_no_args);

    };

  }
}
