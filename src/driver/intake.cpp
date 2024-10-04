#include "../../include/driver/intake.h"

using namespace vex;

IntakeControl::IntakeControl(int speed, vex::controller::button forwardButton, vex::controller::button reverseButton)
{
    this->speed = speed;
    this->forwardButton = forwardButton;
    this->reverseButton = reverseButton;
}

void IntakeControl::intake()
{
    while (this->forwardButton.pressing())
        Intake.spin(vex::directionType::fwd, this->speed, vex::voltageUnits::volt);
}

void IntakeControl::outtake()
{
    while (this->reverseButton.pressing())
        Intake.spin(vex::directionType::rev, this->speed, vex::voltageUnits::volt);
}
