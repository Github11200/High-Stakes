#include "../../include/driver/intake.h"

using namespace vex;

IntakeControl::IntakeControl(int speed, vex::controller::button forwardButton, vex::controller::button reverseButton)
{
    this->speed = speed;
    this->intakeButton = forwardButton;
    this->outtakeButton = reverseButton;
}

void IntakeControl::intake()
{
    while (this->intakeButton.pressing())
        Intake.spin(vex::directionType::fwd, this->speed, vex::voltageUnits::volt);
}

void IntakeControl::outtake()
{
    while (this->outtakeButton.pressing())
        Intake.spin(vex::directionType::rev, this->speed, vex::voltageUnits::volt);
}
