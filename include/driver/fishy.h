#ifndef FISHY_H
#define FISHY_H

#include "vex.h"

using namespace std;
using namespace vex;

class Fishy
{
public:
  // Speed is in VOLTS
  int speed;

  /// @brief Sets the speed of the redirect, and what buttons are used to move it
  /// @param speed Speed is in volts
  Fishy(int speed);

  /// @brief This lifts the redirect
  void liftFishy(bool autonomous = false);

  /// @brief This moves the redirect down
  void lowerFishy();

  /// @brief When the down button is pressed it will say the current position is now 0
  void resetPosition();

  void fishyAutonTask();
};

#endif