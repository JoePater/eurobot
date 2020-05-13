#ifndef FAIRWAYS_H
#define FAIRWAYS_H

#include "geometry.h"
#include "constants.h"


enum FairwayTypes{SMALL_GREEN,SMALL_RED,BIG_GREEN,BIG_RED,NO_FAIRWAY};
std::ostream& operator<<(std::ostream& s,FairwayTypes f);

class System;

class Fairways
{
    Position smallGreen;
    int smallGreenRemaining;
    Position smallRed;
    int smallRedRemaining;
    Position small;

    Position bigGreen;
    int bigGreenRemaining;
    Position bigRed;
    int bigRedRemaining;
    Position big;

    long int lastSignalTime;
public:
    Fairways();
    FairwayTypes getBestFairway(Position currPos,bool isRed);
    Position getFairwayPosition(FairwayTypes type);
    Position getCupPosition(FairwayTypes type);
    bool updateDrive(System* sys,Position currPos,double orientation,FairwayTypes ft); //true if done
    void addBuoys(FairwayTypes ft);
    bool isFree(FairwayTypes ft);
    friend std::ostream& operator<<(std::ostream& s,const Fairways& f);
};

#endif // FAIRWAYS_H
