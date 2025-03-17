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

void MogoControl::reset()
{
  Brain.Screen.clearScreen();
  Brain.Screen.setFillColor(vex::color::red);
  Brain.Screen.drawRectangle(0, 0, 200, 160);
  Clamp.set(false);
  this->mogoState = RELEASED;
}

void MogoControl::autonDelayedClamp(int delay)
{
  static int staticDelay = delay; // Make it static so it persists

  thread clampDelayThread([]()
                          {
                            wait(staticDelay, vex::timeUnits::msec);
                            Clamp.set(true); });
}
