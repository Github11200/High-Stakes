#include "../../include/driver/fishy.h"

using namespace vex;

Fishy::Fishy(int speed)
{
  this->speed = speed;
}

void Fishy::liftFishy()
{
  while (FishyLiftButton.pressing())
  {
    if (FishyMech.position(degrees) > 240)
    {
      FishyMech.stop(brake);
      Intake.stop();
    }
    else
    {
      Intake.spin(vex::directionType::fwd, 6, vex::voltageUnits::volt);
      FishyMech.spin(vex::directionType::fwd, this->speed, vex::voltageUnits::volt);
    }
  }
  while (FishyMech.position(degrees) > 3)
  {
    Intake.spin(vex::directionType::rev, 6, vex::voltageUnits::volt);
    FishyMech.spin(vex::directionType::rev, (FishyMech.position(degrees) / 240) * 12 + 3, vex::voltageUnits::volt);
  }
  Intake.stop();
  FishyMech.stop(hold);
}

void Fishy::lowerFishy()
{
  Intake.spin(vex::directionType::rev, 8, vex::voltageUnits::volt);
  while (FishyLowerButton.pressing())
  {
    if (FishyMech.position(degrees) < -50)
    {
      FishyMech.stop(brake);
      Intake.stop();
    }
    else
    {
      Intake.spin(vex::directionType::rev, 8, vex::voltageUnits::volt);
      FishyMech.spin(vex::directionType::rev, this->speed, vex::voltageUnits::volt);
    }
  }
  Intake.stop();
  FishyMech.stop(brake);
}

void Fishy::resetPosition()
{
  FishyMech.setPosition(0, vex::rotationUnits::deg);
  wait(50, vex::timeUnits::msec);
}