#ifndef REDIRECT_H
#define REDIRECT_H

#include "vex.h"

using namespace std;
using namespace vex;

class RedirectControl
{
private:
    // Speed is in VOLTS
    int speed;
    vex::controller::button redirectUpButton;
    vex::controller::button redirectDownButton;

public:
    /// @brief Sets the speed of the redirect, and what buttons are used to move it
    /// @param speed Speed is in volts
    /// @param redirectUpButton The button that moves the redirect up
    /// @param redirectDownButton The button that moves the redirect down
    RedirectControl(int speed, vex::controller::button redirectUpButton, vex::controller::button redirectDownButton);

    /// @brief This lifts the redirect
    void liftRedirect();

    /// @brief This moves the redirect down
    void lowerRedirect();
};

#endif