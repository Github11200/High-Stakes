#include "../../include/driver/redirect.h"

using namespace vex;

Fishy::Fishy(int speed)
{
  this->speed = speed;
}

void Fishy::liftFishy()
{
  while (FishyLiftButton.pressing()) {
    Intake.spin(vex::directionType::fwd, 8, vex::voltageUnits::volt);
    if(FishyMech.position(degrees) > 350) {
      FishyMech.stop();
    }
    else {
      FishyMech.spin(vex::directionType::fwd, this->speed, vex::voltageUnits::volt);
    }
  }
  Intake.stop();
  FishyMech.stop(hold);
}

void Fishy::lowerFishy()
{
  Intake.spin(vex::directionType::rev, 8, vex::voltageUnits::volt);
  while (FishyLowerButton.pressing()) {
    if(FishyMech.position(degrees) < -50) {
      FishyMech.stop();
    }
    else {
      FishyMech.spin(vex::directionType::rev, this->speed, vex::voltageUnits::volt);
    }
  }
  FishyMech.stop(hold);
}