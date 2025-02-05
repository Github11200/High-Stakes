#include "../../include/driver/descore.h"

using namespace vex;

int DescoreControl::descoreRing()
{
  while (DescoreButton.pressing())
  {
    Hooks.spin(vex::directionType::rev, 4.5, vex::voltageUnits::volt);
    vex::wait(50, vex::timeUnits::msec);
    if (Hooks.velocity(pct) > -5 && DescoreButton.pressing())
    {
      vex::wait(0.5, sec);
      if (Hooks.velocity(pct) > -5 && DescoreButton.pressing())
      {
        Hooks.stop(hold);
        Doinker.set(true);
      }
    }
  }
  Hooks.stop(vex::brakeType::coast);
  Doinker.set(false);

  return 1;
}