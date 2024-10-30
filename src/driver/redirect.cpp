#include "../../include/driver/redirect.h"

using namespace vex;

RedirectControl::RedirectControl(int speed)
{
  this->speed = speed;
}

void RedirectControl::liftRedirect()
{
  while (RedirectLiftButton.pressing()) {
    Redirect.spin(vex::directionType::fwd, this->speed, vex::voltageUnits::volt);
    Intake.spin(vex::directionType::fwd, 8, vex::voltageUnits::volt);
    if(Redirect.position(degrees) > 240) {
      Redirect.stop();
    }
  }
  Intake.stop();
  Redirect.stop(hold);
}

void RedirectControl::lowerRedirect()
{
  while (RedirectLowerButton.pressing()) {
    Redirect.spin(vex::directionType::rev, this->speed, vex::voltageUnits::volt);
    if(Redirect.position(degrees) < 0) {
      Redirect.stop();
    }
  }
  Redirect.stop(hold);
}