#ifndef DESCORE_H
#define DESCORE_H

#include "vex.h"

using namespace vex;
using namespace std;

class DescoreControl
{
private:
    digital_out *DescorePiston;

public:
    DescoreControl();
    int descoreRing();
};

#endif