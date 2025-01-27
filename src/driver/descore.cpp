#include "../../include/driver/descore.h"

using namespace vex;

int DescoreControl::descoreRing()
{
  Hooks.spin(vex::directionType::rev, 5, vex::voltageUnits::volt);
  if (Hooks.velocity(rpm) < 1 && OuttakeButton.pressing())
  {
    vex::wait(2, sec);
    if (Hooks.velocity(rpm) < 1 && OuttakeButton.pressing())
    {
      Intake.stop(coast);
      Hooks.stop(hold);
      Doinker.set(true);
    }
  }

  return 1;
}