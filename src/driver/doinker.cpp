#include "../../include/driver/doinker.h"

using namespace vex;

DoinkerControl::DoinkerControl(vex::controller::button toggleButton)
{
    this->toggleButton = toggleButton;
    this->doinkerState = READY_TO_DOINK;
}

void DoinkerControl::toggle()
{
    if (this->toggleButton.pressing())
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
    }
}