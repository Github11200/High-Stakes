#include "../../include/driver/descore.h"

using namespace vex;

int DescoreControl::descoreRing()
{
  while (DescoreButton.pressing())
  {
    descoreTime += 1;
    cout << descoreTime << endl;
    if (descoreTime < 4)
    {
      cout << "speed" << endl;
      Hooks.spin(vex::directionType::rev, 12, vex::voltageUnits::volt);
    }
    else
    {
      cout << "slow" << endl;
      Hooks.spin(vex::directionType::rev, 4.5, vex::voltageUnits::volt);
    }
    cout << Hooks.velocity(pct) << endl;
    vex::wait(50, vex::timeUnits::msec);
    if (Hooks.velocity(pct) > -5 && DescoreButton.pressing() && descoreTime >= 3)
    {
      cout << "sensed slowness" << endl;
      vex::wait(0.5, sec);
      if (Hooks.velocity(pct) > -5 && DescoreButton.pressing())
      {
        cout << "ahhhh" << endl;
        Hooks.stop(hold);
        Doinker.set(true);
      }
    }
  }
  Hooks.stop(vex::brakeType::coast);
  Doinker.set(false);
  descoreTime = 0;

  return 1;
}