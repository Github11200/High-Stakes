#ifndef DOINKER_H
#define DOINKER_H

#include "vex.h"

using namespace std;
using namespace vex;

enum DoinkerState
{
    DOINKED = true,
    READY_TO_DOINK = false
};

class DoinkerControl
{
private:
    vex::controller::button toggleButton;
    DoinkerState doinkerState;

public:
    /// @brief Controls the doinker either doinking or just staying there :()
    /// @param toggleButton
    DoinkerControl(vex::controller::button toggleButton);

    /// @brief Will toggle between doinked and not doinked based on the state
    void toggle();
};

#endif