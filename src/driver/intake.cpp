#include "../../include/driver/intake.h"

using namespace vex;

IntakeControl::IntakeControl(int speed, int numberOfSecondsToRedirect, vex::controller::button forwardButton, vex::controller::button reverseButton, vex::controller::button redirectButton)
{
    this->speed = speed;
    this->numberOfSecondsToRedirect = numberOfSecondsToRedirect;
    this->intakeButton = forwardButton;
    this->outtakeButton = reverseButton;
    this->redirectButton = redirectButton;
}

void IntakeControl::intake()
{
    while (this->intakeButton.pressing())
        Intake.spin(vex::directionType::fwd, this->speed, vex::voltageUnits::volt);
}

void IntakeControl::intakeToRedirect()
{
    // Intake the ring until it reaches the limit switch
    while (!RedirectLimitSwitch.pressing())
        Intake.spin(vex::directionType::fwd, this->speed, vex::voltageUnits::volt);

    // Now that it's at the top, spin it in reverse to put it into the redirect
    Intake.spinFor(vex::directionType::rev, 2, vex::timeUnits::sec);

    vex::wait(20, vex::timeUnits::msec);
}

void IntakeControl::outtake()
{
    while (this->outtakeButton.pressing())
        Intake.spin(vex::directionType::rev, this->speed, vex::voltageUnits::volt);
}
