#ifndef MOGO_H
#define MOGO_H

#include "vex.h"

using namespace vex;
using namespace std;

enum MogoState
{
    CLAMPED = true,
    RELEASED = false
};

class MogoControl
{
public:
    vex::controller::button toggleButton;
    MogoState mogoState;

    /// @brief Controls the pistons to hold a mogo at the back of the robot
    /// @param clampButton When this button is pressed the mogo is clamped onto
    /// @param releaseButton When this button is pressed the mogo is let go of
    MogoControl(vex::controller::button toggleButton);

    /// @brief Will toggle between clamped and released based on current state
    void toggle();
};

#endif