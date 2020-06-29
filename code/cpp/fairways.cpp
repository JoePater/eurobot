#include "fairways.h"
#include "constants.h"
#include "system.h"
#include <cmath>
/*
enum FairwaysFlags{
    SMALL_GREEN_FULL=1,
    SMALL_RED_FULL=2,
    BIG_GREEN_FULL=4,
    BIG_RED_FULL=8
};
*/
const constexpr double pi = std::atan(1)*4;

const double fairway_tolerance = 25;
const double fairway_ang_tol = 15;
const long min_motor_signal_period = 50;

const double slow_speed = 0.5;
const float rotate_speed = 50;
const double angle_rotate_limit = 20;
const double rotation_coefficient = 0.4; //for each mm off target, value to decrease PWM by

const double buoy_separation = 90;

std::ostream& operator<<(std::ostream& s,FairwayTypes f){
    switch(f){
        case SMALL_RED: return s<<"small red";
        case SMALL_GREEN: return s<<"small green";
        case BIG_RED: return s<<"big red";
        case BIG_GREEN: return s<<"big green";
        case NO_FAIRWAY: return s<<"none";
    }
    return s<<"error";
}

Fairways::Fairways():
    smallGreen(team==BLUE?Position(1900,500):Position(1900,2500)),
    smallRed(team==BLUE?Position(1900,600):Position(1900,2400)),
    small(team==BLUE?Position(1700,550):Position(1700,2450)),
    bigGreen(team==BLUE?Position(900,100):Position(900,2900)),
    bigRed(team==BLUE?Position(1100,100):Position(1100,2900)),
    big(team==BLUE?Position(1000,300):Position(1000,270000)),
    smallGreenRemaining(4),smallRedRemaining(4),
    bigGreenRemaining(6),bigRedRemaining(6),
    lastSignalTime(0){}

FairwayTypes Fairways::getBestFairway(Position currPos,bool isRed){
    if(isRed){
        //big closer than small
        if(getDistance(currPos,big) < getDistance(currPos,small)){
            if(bigRedRemaining > 0) return BIG_RED;
            else if(smallRedRemaining > 0) return SMALL_RED;
            else if(bigGreenRemaining > 0) return BIG_GREEN;
            else if(smallGreenRemaining > 0) return SMALL_GREEN;
            else return NO_FAIRWAY;
        }else{
            if(smallRedRemaining > 0) return SMALL_RED;
            else if(bigRedRemaining > 0) return BIG_RED;
            else if(smallGreenRemaining > 0) return SMALL_GREEN;
            else if(bigGreenRemaining > 0) return BIG_GREEN;
            else return NO_FAIRWAY;
        }
    }else{
        //big closer than small
        if(getDistance(currPos,big) < getDistance(currPos,small)){
            if(bigGreenRemaining > 0) return BIG_GREEN;
            else if(smallGreenRemaining > 0) return SMALL_GREEN;
            else if(bigRedRemaining > 0) return BIG_RED;
            else if(smallRedRemaining > 0) return SMALL_RED;
            else return NO_FAIRWAY;
        }else{
            if(smallGreenRemaining > 0) return SMALL_GREEN;
            else if(bigGreenRemaining > 0) return BIG_GREEN;
            else if(smallRedRemaining > 0) return SMALL_RED;
            else if(bigRedRemaining > 0) return BIG_RED;
            else return NO_FAIRWAY;
        }
    }
}

Position Fairways::getFairwayPosition(FairwayTypes type){
    switch(type){
    case SMALL_GREEN:
    case SMALL_RED:
        return small;
    case BIG_GREEN:
    case BIG_RED:
        return big;
    default:
        return big;
    }
}

bool Fairways::updateDrive(System* sys,Position currPos,double orientation,FairwayTypes ft){
    Position targetPos(0,0);
    switch(ft){
    case SMALL_GREEN: targetPos=smallGreen; break;
    case SMALL_RED: targetPos=smallRed; break;
    case BIG_GREEN: targetPos=bigGreen; break;
    case BIG_RED: targetPos=bigRed; break;
    }
    targetPos = Line(currPos,targetPos).getPoint(distance_to_claw);

    bool rotating = false;
    if(getDistance(currPos,targetPos)<fairway_tolerance) rotating=true;

    Mechanisms& mech = sys->getRobot().mechanisms;

    if(rotating){
        double targetBearing = getAngle(currPos,targetPos);
        double angleOff = targetBearing-orientation;
        if(std::abs(angleOff)<fairway_ang_tol){
            return true;
        }
        if(sys->getTime()-lastSignalTime > min_motor_signal_period){
            float x = angleOff > 0 ? rotate_speed : -rotate_speed;
            mech.setDrive(x,-x);
            lastSignalTime = sys->getTime();
        }
        return false;
    }else{
        double angleOff = orientation - getAngle(currPos,targetPos);
        //probably after arriving at a point
        if(angleOff > angle_rotate_limit){
            mech.setDrive(rotate_speed,-rotate_speed); //clockwise
        }else if(angleOff < -angle_rotate_limit){
            mech.setDrive(-rotate_speed,rotate_speed); //anticlockwise
        }else{
            double distOff = std::abs(getDistance(currPos,targetPos) * std::tan(pi/180*angleOff));
            double handicap = distOff*rotation_coefficient;
            handicap = handicap <= 100 ? handicap : 100; //make sure it's <= 100

            if(angleOff >= 0){
                mech.setDrive(100*slow_speed,(100 - handicap)*slow_speed);//clockwise
            }else{
                mech.setDrive((100 - handicap)*slow_speed,100*slow_speed);//anticlockwise
            }
        }
    }
    return false;
}

void Fairways::addBuoys(FairwayTypes ft){
    switch(ft){
        case SMALL_GREEN: smallGreen.x-=buoy_separation; smallGreenRemaining--; break;
        case SMALL_RED: smallRed.x-=buoy_separation; smallRedRemaining--; break;
        case BIG_GREEN:
            if(team==BLUE) bigGreen.y+=buoy_separation;
            else bigGreen.y-=buoy_separation;
            bigGreenRemaining--; break;
        case BIG_RED:
            if(team==BLUE) bigRed.y+=buoy_separation;
            else bigRed.y-=buoy_separation;
            bigRedRemaining--; break;
    }
}
bool Fairways::isFree(FairwayTypes ft){
    switch(ft){
        case SMALL_GREEN: return smallGreenRemaining>0;
        case SMALL_RED: return smallRedRemaining>0;
        case BIG_GREEN: return bigGreenRemaining>0;
        case BIG_RED: return bigRedRemaining>0;
        default: return false;
    }
}
Position Fairways::getCupPosition(FairwayTypes ft){
    switch(ft){
        case SMALL_GREEN: return smallGreen;
        case SMALL_RED: return smallRed;
        case BIG_GREEN: return bigGreen;
        case BIG_RED: return bigRed;
        default: return bigRed;
    }
}

std::ostream& operator<<(std::ostream& s,const Fairways& f){
    s<<"Remaining spaces, positions:\n";
    s<<"Small red:green "<<f.smallRedRemaining<<","<<f.smallRed<<":";
    s<<f.smallGreenRemaining<<","<<f.smallGreen<<"\n";
    s<<"Big red:green "<<f.bigRedRemaining<<","<<f.bigRed<<":";
    return s<<f.bigGreenRemaining<<","<<f.bigGreen;
}


