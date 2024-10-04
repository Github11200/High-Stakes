#ifndef INTAKE_H
#define INTAKE_H

#include "vex.h"

using namespace vex;
using namespace std;

class IntakeControl
{
private:
    // Speed is in volts
    int speed;
    vex::controller::button forwardButton;
    vex::controller::button reverseButton;

public:
    /// @brief Sets the speed, button to turn the intake forward, and button to turn it in reverse
    /// @param speed The speed in VOLTS
    /// @param forwardButton Spins the motor to intake a ring
    /// @param reverseButton Spins the motor to outtake a ring
    IntakeControl(int speed, vex::controller::button intakeButton, vex::controller::button outtakeButton);

    /// @brief Spin the intake to take in a ring
    void intake();

    /// @brief Spin the intake to outtake the ring or put it into the redirect
    void outtake();
};

#endif