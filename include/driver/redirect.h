#ifndef REDIRECT_H
#define REDIRECT_H

#include "vex.h"

using namespace std;
using namespace vex;

class RedirectControl
{
public:
  // Speed is in VOLTS
  int speed;

  /// @brief Sets the speed of the redirect, and what buttons are used to move it
  /// @param speed Speed is in volts
  RedirectControl(int speed);

  /// @brief This lifts the redirect
  void liftRedirect();

  /// @brief This moves the redirect down
  void lowerRedirect();
};

#endif