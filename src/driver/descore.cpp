#include "../../include/driver/descore.h"

using namespace vex;

DescoreControl::DescoreControl()
{
  // Rename the doinker piston :)
  this->DescorePiston = &Doinker;
}

int DescoreControl::descoreRing()
{
  while (DescoreButton.pressing())
  {
    if (Intake.velocity(vex::percentUnits::pct) > 10)
      Intake.spin(vex::directionType::rev, 12, vex::voltageUnits::volt);
    else
    {
      this->DescorePiston->set(true);
      wait(0.5, vex::timeUnits::sec);
      this->DescorePiston->set(false);
    }
    wait(20, vex::timeUnits::msec);
  }

  return 1;
}