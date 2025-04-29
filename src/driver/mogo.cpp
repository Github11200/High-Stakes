#include "../../include/driver/mogo.h"

using namespace vex;

MogoControl::MogoControl()
{
  this->mogoState = RELEASED;
}

void MogoControl::toggle()
{
  if (ClampButton.pressing())
  {
    if (this->mogoState == RELEASED)
    {
      Clamp.set(true);
      this->mogoState = CLAMPED;
      Brain.Screen.clearScreen();
      Brain.Screen.setFillColor(vex::color::green);
      Brain.Screen.drawRectangle(0, 0, 200, 160);
    }
    else
    {
      Clamp.set(false);
      this->mogoState = RELEASED;
      Brain.Screen.clearScreen();
      Brain.Screen.setFillColor(vex::color::red);
      Brain.Screen.drawRectangle(0, 0, 200, 160);
    }
    while (ClampButton.pressing())
    {
      wait(25, vex::timeUnits::msec);
    }
  }
}

void MogoControl::autonDelayedClamp(int delay)
{
  wait(delay, vex::timeUnits::msec);
  Clamp.set(true);
}

void MogoControl::mogoAutonTask()
{
  if (mogoClamp) // the variable that the autonomous sets to true to spin the intake
  {
    autonDelayedClamp(mogoClampDelay);
    mogoClamp = false;
  }
}
