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

      SPOOKSHOW_MAKE_MOCK_METHOD_0(void, void_no_args);
      SPOOKSHOW_MAKE_MOCK_METHOD_1(void, void_one_arg, int);
      SPOOKSHOW_MAKE_MOCK_METHOD_0(int, int_no_args);
      SPOOKSHOW_MAKE_MOCK_METHOD_1(int, int_one_arg, int);

    };

  }
}
