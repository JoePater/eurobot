#include "motionmodel.h"

MotionModel::MotionModel():timeSeen(-1),vx(0),vy(0){
    lastSeen = Position(/*...*/);
}

MotionModel::MotionModel(Position p):timeSeen(0),vx(0),vy(0),lastSeen(p){}

void MotionModel::reportSighting(Position p,long int currTime){
    double xMoved = p.x-lastSeen.x;
    double yMoved = p.y-lastSeen.y;
    long int timePassed = currTime-timeSeen;
    vx = xMoved/timePassed;
    vy = yMoved/timePassed;

    lastSeen = p;
    timeSeen = currTime;
}

Position MotionModel::estimatePosition(long int currTime){
    long int timePassed = currTime - timeSeen;
    Position now = lastSeen + Position(vx*timePassed,vy*timePassed);
    return now;
}













//
