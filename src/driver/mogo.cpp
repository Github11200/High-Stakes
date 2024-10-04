#include "../../include/driver/mogo.h"

using namespace vex;

MogoControl::MogoControl(vex::controller::button toggleButton)
{
    this->toggleButton = toggleButton;
    this->mogoState = RELEASED;
}

void MogoControl::toggle()
{
    if (this->toggleButton.pressing())
    {
        if (this->mogoState == RELEASED)
        {
            Clamp.set(true);
            this->mogoState = CLAMPED;
        }
        else
        {
            Clamp.set(false);
            this->mogoState = RELEASED;
        }

        wait(25, vex::timeUnits::msec);
    }
}