#ifndef INTAKE_H
#define INTAKE_H

#include "vex.h"

using namespace vex;
using namespace std;

class IntakeControl
{
public:
    // Speed is in volts
    int speed;
    int numberOfSecondsToRedirect;
    vex::controller::button intakeButton;
    vex::controller::button outtakeButton;
    vex::controller::button redirectButton;

    /// @brief Sets the speed, button to turn the intake forward, and button to turn it in reverse
    /// @param speed The speed in VOLTS
    /// @param forwardButton Spins the motor to intake a ring
    /// @param reverseButton Spins the motor to outtake a ring
    IntakeControl(int speed, int numberOfSecondsToRedirect, vex::controller::button intakeButton, vex::controller::button outtakeButton, vex::controller::button redirectButton);

    /// @brief Spin the intake to take in a ring
    void intake();

    /// @brief This will intake the ring, and the automatically spin the intake in reverse to put it into the redirect
    void intakeToRedirect();

    /// @brief Spin the intake to outtake the ring or put it into the redirect
    void outtake();
};

#endif