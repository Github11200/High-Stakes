#include "../../include/driver/joystick.h"

using namespace std;
using namespace vex;

Joystick::Joystick(double deadband, double driveSlew, double negativeInertiaScalar, double sensitivity)
{
    this->DRIVE_DEADBAND = deadband;
    this->DRIVE_SLEW = driveSlew;
    this->NEGATIVE_INERTIA_SCALAR = negativeInertiaScalar;
    this->SENSITIVITY = sensitivity;
}

double Joystick::calculateBezierOutput(double value)
{
    // https://www.desmos.com/calculator/cpksupohg8
    double result = 0;
    double bezierConstants[4] = {0, 0.736, -0.43, 1};

    result += pow((1 - value), 3) * bezierConstants[0];
    result += 3 * value * pow((1 - value), 2) * bezierConstants[1];
    result += 3 * pow(value, 2) * (1 - value) * bezierConstants[2];
    result += pow(value, 3) * bezierConstants[3];

    return result;
}

void Joystick::updateAccumulators()
{
    if (this->negativeInertiaAccumulator > 1)
        this->negativeInertiaAccumulator -= 1;
    else if (this->negativeInertiaAccumulator < -1)
        this->negativeInertiaAccumulator += 1;
    else
        this->negativeInertiaAccumulator = 0;
}

pair<double, double> Joystick::cheesy()
{
    bool turnInPlace = false;
    double throttle = Controller.Axis1.position();
    double turn = Controller.Axis2.position();
    double left = 0;
    double right = 0;

    // If the controller joystick values are really small movements then we'll just turn in place
    if (!deadband(throttle, this->DRIVE_DEADBAND) && !deadband(turn, this->DRIVE_DEADBAND))
    {
        turnInPlace = true;
        throttle = 0;
    }
    else if (throttle - this->previousThrottle > this->DRIVE_SLEW)
        throttle += this->previousThrottle + DRIVE_SLEW;
    else if (throttle - this->previousThrottle < -(DRIVE_SLEW * 2))
        throttle += this->previousThrottle - (2 * DRIVE_SLEW);

    // Remap the turn
    turn = this->calculateBezierOutput(turn);

    if (turnInPlace)
    {
        // I hope these random links help :)
        // https://www.desmos.com/calculator/7nef4ospav, we are cubing the turn value for more control
        left = pow(turn, 3);
        right = pow(-turn, 3);
    }
    else
    {
        // The reverse integral controller to reduce turning power depending on how long you've turned for
        double negativeInertiaPower = (throttle - this->previousThrottle) * this->NEGATIVE_INERTIA_SCALAR;
        this->negativeInertiaAccumulator += negativeInertiaPower;

        turn = abs(throttle) *                             // The faster the throttle, the faster we turn
               (this->negativeInertiaAccumulator + turn) * // Add the accumulator to the original turn value
               this->SENSITIVITY;

        left += throttle + turn;
        right -= throttle + turn;

        updateAccumulators();
    }

    this->previousThrottle = throttle;
    return pair<double, double>(left, right);
}