#include "../../include/driver/redirect.h"

using namespace vex;

RedirectControl::RedirectControl(int speed, vex::controller::button redirectUpButton, vex::controller::button redirectDownButton)
{
    this->speed = speed;
    this->liftRedirectButton = redirectUpButton;
    this->redirectLowerButton = redirectDownButton;
}

void RedirectControl::liftRedirect()
{
    while (this->liftRedirectButton.pressing())
        Redirect.spin(vex::directionType::fwd, this->speed, vex::voltageUnits::volt);
}

void RedirectControl::lowerRedirect()
{
    while (this->redirectLowerButton.pressing())
        Redirect.spin(vex::directionType::rev, this->speed, vex::voltageUnits::volt);
}