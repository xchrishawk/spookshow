/**
 * @file	main.cpp
 * @author	Chris Vig (chris@invictus.so)
 * @date	2017/01/05
 */

/* -- Includes -- */

#include <iostream>
#include <string>

#include <gtest/gtest.h>
#include <spookshow/spookshow.hpp>

/* -- Namespaces -- */

using namespace spookshow;
using namespace testing;

/* -- Mocked Object -- */

/**
 * This is the class that we will be mocking to demonstrate Spookshow.
 *
 * The requirements are:
 * - Class must not be `final`, since we will need to subclass it when we create the mock.
 * - Any method you wish to mock must be `virtual`.
 */
class airplane
{
public:

  airplane(const std::string& tail_number)
    : m_tail_number(tail_number)
  { }

  virtual ~airplane() = default;

  std::string tail_number() const { return m_tail_number; }

  virtual double get_airspeed() const { return m_airspeed; }
  virtual void set_airspeed(double airspeed) { m_airspeed = airspeed; }

  virtual double get_altitude() const { return m_altitude; }
  virtual void set_altitude(double altitude) { m_altitude = altitude; }

  virtual bool get_gear_extended() const { return m_gear_extended; }
  virtual void set_gear_extended(bool extended) { m_gear_extended = extended; }

  virtual void takeoff() { std::cout << m_tail_number << " took off!" << std::endl; }
  virtual void land() { std::cout << m_tail_number << " landed!" << std::endl; }

private:
  std::string m_tail_number;
  double m_airspeed;
  double m_altitude;
  bool m_gear_extended;
};

/* -- Mock Object -- */

/**
 * This the mock object we will use for testing.
 **/
class airplane_mock : public airplane
{
public:

  airplane_mock(const std::string& tail_number)
    : airplane(tail_number)
  { }

  // Mock versions of inherited methods are created using the SPOOKSHOW_MOCK_* macros.
  SPOOKSHOW_MOCK_CONST_METHOD_0(double, get_airspeed);
  SPOOKSHOW_MOCK_METHOD_1(void, set_airspeed, double);
  SPOOKSHOW_MOCK_CONST_METHOD_0(double, get_altitude);
  SPOOKSHOW_MOCK_METHOD_1(void, set_altitude, double);
  SPOOKSHOW_MOCK_CONST_METHOD_0(bool, get_gear_extended);
  SPOOKSHOW_MOCK_METHOD_1(void, set_gear_extended, bool);
  SPOOKSHOW_MOCK_METHOD_0(void, takeoff);
  SPOOKSHOW_MOCK_METHOD_0(void, land);

};

/* -- Class Under Test -- */

/**
 * This is the class we will be testing. It interfaces with the `airplane` class.
 */
class pilot
{
public:

  /** Takes off an `airplane` object. */
  void takeoff_airplane(airplane& airplane) const
  {
    airplane.takeoff();
    airplane.set_gear_extended(false);
  }

  /** Lands an `airplane` object. This method has a bug. */
  void land_airplane(airplane& airplane) const
  {
    airplane.set_gear_extended(false);
    airplane.land();
  }


  /** Reports the airspeed for an `airplane` object. */
  double report_airspeed(const airplane& airplane) const
  {
    return airplane.get_airspeed();
  }

};

/* -- Main Method -- */

/**
 * Main entry point for the application.
 */
int main(int argc, char** argv)
{
  // First, initialize Google Tests.
  InitGoogleTest(&argc, argv);

  // We need to set a failure handler for the Spookshow library. A simple lambda calling the
  // ADD_FAILURE() macro is good enough for this purpose, although you may add additional handling
  // as needed.
  spookshow::set_fail_handler([] (const std::string& message) {
      ADD_FAILURE() << message;
    });

  // Finally, let Google Tests run all of the test cases.
  return RUN_ALL_TESTS();
}

/* -- Unit Tests -- */

class PilotTest : public Test
{
protected:

  // Create the pilot and airplane objects we'll use for testing.
  pilot m_pilot;
  airplane_mock m_airplane { "N123CV" };

};

TEST_F(PilotTest, TakeOffAirplane)
{
  // Order is important for this test case (we need to take off before we can retract the gear).
  expectation_order order;

  // Create expectations for the methods we want to verify are called.
  expectation took_off("takeoff() called");
  expectation gear_retracted("set_gear_retracted(false) called");

  // Now set up our mocks.
  SPOOKSHOW(m_airplane, set_gear_extended)
    .once(noops())
    .requires(arg_eq<0>(false))
    .fulfills(gear_retracted);
  SPOOKSHOW(m_airplane, takeoff)
    .once(noops())
    .fulfills(took_off);

  // Run the test.
  m_pilot.takeoff_airplane(m_airplane);
}


TEST_F(PilotTest, LandAirplane)
{
  // This is similar to the above test case, but it should uncover a bug.
  // In addition, we use the convenience macro `SPOOKSHOW_EXPECT_ONCE` to create a unique
  // expectation and tie it to a functor call with a single line of code.
  expectation_order order;
  SPOOKSHOW_EXPECT_ONCE(m_airplane, set_gear_extended, noops()).requires(arg_eq<0>(true));
  SPOOKSHOW_EXPECT_ONCE(m_airplane, land, noops());

  m_pilot.land_airplane(m_airplane);
}

TEST_F(PilotTest, ReportAirspeed)
{
  // For this test case, we don't care about expectations, we just want to make sure the `pilot`
  // object returns the correct value.
  static const double EXPECTED_AIRSPEED = 200.0;

  // Set up the method, then verify the `pilot` object returns the correct value.
  SPOOKSHOW(m_airplane, get_airspeed).once(returns(EXPECTED_AIRSPEED));
  EXPECT_EQ(m_pilot.report_airspeed(m_airplane), EXPECTED_AIRSPEED);
}
