#include "../../include/driver/fishy.h"

using namespace vex;

Fishy::Fishy(int speed)
{
  this->speed = speed;
}

void Fishy::liftFishy(bool autonomous)
{
  timer time = timer();
  while (FishyLiftButton.pressing() || autonomous)
  {
    fishing = true;
    if (time.value() > 1 && !autonomous)
    {
      if (FishyMech.position(degrees) < 180)
      {
        Intake.spin(vex::directionType::fwd, 6, vex::voltageUnits::volt);
        time = 0; // give it some time to score the ring
      }
    }
    if (time.value() > 2 && autonomous)
    {
      if (FishyMech.position(degrees) > 180)
      { // it's probably stuck on the Wall Stake, so just give up
        break;
      }
      else
      { // it's probably stuck lower down, on the intake, so you can spin the intake a bit
        Intake.spin(vex::directionType::fwd, 6, vex::voltageUnits::volt);
        time = 0; // give it some time to score the ring
      }
    }
    if (FishyMech.position(degrees) > 230)
    {
      FishyMech.stop(brake);
      Intake.stop();
      if (autonomous)
      {
        wait(100, vex::timeUnits::msec);
        break;
      }
    }
    else
    {
      // Intake.spin(vex::directionType::fwd, 6, vex::voltageUnits::volt);
      FishyMech.spin(vex::directionType::fwd, this->speed, vex::voltageUnits::volt);
    }
    wait(10, vex::timeUnits::msec);
  }
  while (FishyMech.position(degrees) > 3)
  {
    // Intake.spin(vex::directionType::rev, 6, vex::voltageUnits::volt);
    FishyMech.spin(vex::directionType::rev, (FishyMech.position(degrees) / 240) * 12 + 3, vex::voltageUnits::volt);
    wait(10, vex::timeUnits::msec);
  }
  Intake.stop();
  FishyMech.stop(hold);
  fishing = false;
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
    wait(10, vex::timeUnits::msec);
  }
  Intake.stop();
  FishyMech.stop(brake);
}

void Fishy::resetPosition()
{
  FishyMech.setPosition(0, vex::rotationUnits::deg);
  wait(50, vex::timeUnits::msec);
}

void Fishy::fishyAutonTask()
{
  if (pre_driver)
  {
    if (raiseFishy)
    {
      if (FishyMech.position(degrees) > 240)
      {
        FishyMech.stop(brake);
      }
      else
      {
        Intake.spin(vex::directionType::fwd, 6, vex::voltageUnits::volt);
        FishyMech.spin(vex::directionType::fwd, this->speed, vex::voltageUnits::volt);
      }
    }
    else if (!intakeSort)
    {
      if (FishyMech.position(degrees) > 3)
      {
        Intake.spin(vex::directionType::rev, 6, vex::voltageUnits::volt);
        FishyMech.spin(vex::directionType::rev, (FishyMech.position(degrees) / 240) * 12 + 3, vex::voltageUnits::volt);
        wait(10, vex::timeUnits::msec);
      }
      else
      {
        FishyMech.stop(hold);
        Intake.stop();
      }
    }
  }
}