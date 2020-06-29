#ifndef ROBOT_H
#define ROBOT_H

#include "mechanisms.h"
#include "geometry.h"
#include "constants.h"

class Robot{
public:
    Mechanisms mechanisms;
    Position coordinates;
    double orientation; //away from big beacon is 0

    Robot():coordinates(team==BLUE?Position(800,200):Position(800,2800)){};
};









#endif
