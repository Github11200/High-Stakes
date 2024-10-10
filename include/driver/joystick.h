#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "vex.h"

using namespace vex;
using namespace std;

class Joystick
{
private:
  double calculateBezierOutput(double value);
  void updateAccumulators();

public:
  double DRIVE_DEADBAND = 0;
  double DRIVE_SLEW = 0.02;
  double NEGATIVE_INERTIA_SCALAR = 0;
  double SENSITIVITY = 0;
  double previousThrottle = 0;
  double negativeInertiaAccumulator = 0;

  Joystick(double deadband, double driveSlew, double negativeInertiaSclar, double sensitivity);

  pair<double, double> cheesy();
};

#endif