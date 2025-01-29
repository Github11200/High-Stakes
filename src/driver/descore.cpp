#include "../../include/driver/descore.h"

using namespace vex;

int DescoreControl::descoreRing()
{
  while (DescoreButton.pressing())
  {
    Hooks.spin(vex::directionType::rev, 5, vex::voltageUnits::volt);
    wait(10, vex::timeUnits::msec);
    if (Hooks.velocity(rpm) < 1 && DescoreButton.pressing())
    {
      vex::wait(2, sec);
      if (Hooks.velocity(rpm) < 1 && DescoreButton.pressing())
      {
        Hooks.stop(hold);
        Doinker.set(true);
      }
    }
  }
  wait(10, vex::timeUnits::msec);
  Intake.stop(vex::brakeType::coast);
  Hooks.stop(vex::brakeType::coast);
  Doinker.set(false);

  return 1;
}