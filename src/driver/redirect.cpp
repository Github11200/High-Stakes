#include "../../include/driver/redirect.h"

using namespace vex;

RedirectControl::RedirectControl(int speed)
{
  this->speed = speed;
}

void RedirectControl::liftRedirect()
{
  while (RedirectLiftButton.pressing())
    Redirect.spin(vex::directionType::rev, this->speed, vex::voltageUnits::volt);
  Redirect.stop(hold);
}

void RedirectControl::lowerRedirect()
{
  while (RedirectLowerButton.pressing())
    Redirect.spin(vex::directionType::fwd, this->speed, vex::voltageUnits::volt);
  Redirect.stop(hold);
}