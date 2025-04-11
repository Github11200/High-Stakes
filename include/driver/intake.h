#ifndef INTAKE_H
#define INTAKE_H

#include "vex.h"

using namespace vex;
using namespace std;

class IntakeControl
{
public:
  /// @brief Checks if a ring is in the intake and if it's the wrong color
  /// @return True or false if it's the wrong color and there's a ring
  bool shouldEjectRing();

  /// @brief Throws the ring out of the intake because it's the wrong one
  void ejectRing();

  // Speed is in volts
  int speed;

  /// @brief Sets the speed, button to turn the intake forward, and button to turn it in reverse
  /// @param speed The speed in VOLTS
  IntakeControl(int speed);

  /// @brief Spin the intake to take in a ring
  void intake();

  /// @brief This will intake the ring, and stop it in time for the lady brown
  void intakeToLadyBrown();

  /// @brief Spin the intake to outtake the ring or put it into the redirect
  void outtake();

  int hue_difference(int hue1, int hue2);

  void intakeAutonTask();
};

#endif