#include "../../include/driver/doinker.h"

using namespace vex;
using namespace std;

DoinkerControl::DoinkerControl()
{
  // this->doinkerState = READY_TO_DOINK;
}

void DoinkerControl::toggle()
{
  if (DoinkerButton.pressing())
  {
    if (this->doinkerState == true)
    {
      Doinker.set(true);
      this->doinkerState = false;
    }
    else
    {
      Doinker.set(false);
      this->doinkerState = false;
    }

    wait(25, vex::timeUnits::msec);
  }
}