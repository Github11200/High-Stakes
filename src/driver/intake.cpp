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
  // // Wait till the ring hits the limit switch, this means it's close to being in position
  // while (!RedirectLimitSwitch.pressing() && IntakeToRedirectButton.pressing())
  //   Intake.spin(vex::directionType::fwd, 12, vex::voltageUnits::volt);

  // // Move the ring slower to it's position so that it doesn't fly off
  // while (RedirectLimitSwitch.pressing() && IntakeToRedirectButton.pressing())
  //   Intake.spinFor(vex::directionType::fwd, 2, vex::rotationUnits::rev, 40, vex::velocityUnits::pct, true);

  // Intake.stop(vex::brakeType::coast);

  // vex::wait(20, vex::timeUnits::msec);
}

void IntakeControl::outtake()
{
  while (OuttakeButton.pressing())
    Intake.spin(vex::directionType::rev, this->speed, vex::voltageUnits::volt);
  Intake.stop(vex::brakeType::coast);
}
