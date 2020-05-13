#ifndef MOTIONMODEL_H
#define MOTIONMODEL_H

class MotionModel{
    Position lastSeen;
    long int timeSeen;
    float vx,vy;
public:
    MotionModel();
    MotionModel(Position p);
    void reportSighting(Position p,long int currTime);
    Position estimatePosition(long int currTime);
};













#endif
