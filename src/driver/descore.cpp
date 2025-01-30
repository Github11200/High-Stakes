#include "../../include/driver/descore.h"

using namespace vex;

int DescoreControl::descoreRing()
{
  while (DescoreButton.pressing())
  {
    cout << "yuibbu" << endl;
    Hooks.spin(vex::directionType::rev, 5, vex::voltageUnits::volt);
    wait(50, vex::timeUnits::msec);
    if (Hooks.velocity(pct) < 5 && DescoreButton.pressing())
    {
      cout << "doihsuowacbix" << endl;
      vex::wait(2, sec);
      if (Hooks.velocity(pct) < 5 && DescoreButton.pressing())
      {
        cout << "ahhhh" << endl;
        Hooks.stop(hold);
        Doinker.set(true);
      }
    }
  }
  Hooks.stop(vex::brakeType::coast);
  Doinker.set(false);

  return 1;
}