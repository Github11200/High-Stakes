#include "../../include/driver/redirect.h"

using namespace vex;

Fishy::Fishy(int speed)
{
  this->speed = speed;
}

void Fishy::liftFishy()
{
  while (FishyLiftButton.pressing())
  {
    if (FishyMech.position(degrees) > 300)
    {
      FishyMech.stop(brake);
      Intake.stop();
    }
    else
    {
      Intake.spin(vex::directionType::fwd, 8, vex::voltageUnits::volt);
      FishyMech.spin(vex::directionType::fwd, this->speed, vex::voltageUnits::volt);
    }
  }
  while (FishyMech.position(degrees) > 3)
  {
    Intake.spin(vex::directionType::rev, 8, vex::voltageUnits::volt);
    FishyMech.spin(vex::directionType::rev, this->speed, vex::voltageUnits::volt);
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