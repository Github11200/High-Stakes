#include "../../include/driver/redirect.h"

using namespace vex;

RedirectControl::RedirectControl(int speed)
{
  this->speed = speed;
}

void RedirectControl::liftRedirect()
{
  while (RedirectLiftButton.pressing()) {
    Intake.spin(vex::directionType::fwd, 8, vex::voltageUnits::volt);
    if(Redirect.position(degrees) > 350) {
      Redirect.stop();
    }
    else {
      Redirect.spin(vex::directionType::fwd, this->speed, vex::voltageUnits::volt);
    }
  }
  Intake.stop();
  Redirect.stop(hold);
}

void RedirectControl::lowerRedirect()
{
  Intake.spin(vex::directionType::rev, 8, vex::voltageUnits::volt);
  while (RedirectLowerButton.pressing()) {
    if(Redirect.position(degrees) < 0) {
      Redirect.stop();
    }
    else {
      Redirect.spin(vex::directionType::rev, this->speed, vex::voltageUnits::volt);
    }
  }
  Redirect.stop(hold);
}