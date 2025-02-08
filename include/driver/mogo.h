#ifndef MOGO_H
#define MOGO_H

#include "vex.h"

using namespace vex;
using namespace std;

enum MogoState
{
  CLAMPED = true,
  RELEASED = false
};

class MogoControl
{
public:
  MogoState mogoState;

  /// @brief Controls the pistons to hold a mogo at the back of the robot
  MogoControl();

  /// @brief Will toggle between clamped and released based on current state
  void toggle();

  /// @brief Resets it back to the up position
  void reset();
};

#endif