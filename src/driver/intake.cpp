#include "../../include/driver/intake.h"

using namespace vex;

IntakeControl::IntakeControl(int speed, int numberOfSecondsToRedirect)
{
  this->speed = speed;
  this->numberOfSecondsToRedirect = numberOfSecondsToRedirect;
}

void IntakeControl::intake()
{
  while (IntakeButton.pressing())
    Intake.spin(vex::directionType::fwd, this->speed, vex::voltageUnits::volt);
  Intake.stop(vex::brakeType::coast);
}

void IntakeControl::intakeToRedirect()
{
  // // Intake the ring until it reaches the limit switch
  // while (!RedirectLimitSwitch.pressing())
  //     Intake.spin(vex::directionType::fwd, this->speed, vex::voltageUnits::volt);

  // // Now that it's at the top, spin it in reverse to put it into the redirect
  // Intake.spinFor(vex::directionType::rev, 2, vex::timeUnits::sec);

  // vex::wait(20, vex::timeUnits::msec);
}

void IntakeControl::outtake()
{
  while (OuttakeButton.pressing())
    Intake.spin(vex::directionType::rev, this->speed, vex::voltageUnits::volt);
  Intake.stop(vex::brakeType::coast);
}
