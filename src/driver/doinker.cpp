#include "../../include/driver/doinker.h"

using namespace vex;
using namespace std;

DoinkerControl::DoinkerControl()
{
  this->doinkerState = READY_TO_DOINK;
}

void DoinkerControl::toggle()
{
  if (DoinkerButton.pressing())
  {
    if (this->doinkerState == READY_TO_DOINK)
    {
      Doinker.set(true);
      this->doinkerState = DOINKED;
    }
    else
    {
      Doinker.set(false);
      this->doinkerState = READY_TO_DOINK;
    }

    wait(25, vex::timeUnits::msec);
  }
}