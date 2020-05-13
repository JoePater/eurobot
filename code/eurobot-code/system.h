#ifndef SYSTEM_H
#define SYSTEM_H

#include "robot.h"
#include "route.h"
#include "mechanisms.h"
#include "fairways.h"
#include "arena.h"
#include "geometry.h"
#include "scheduledevent.h"

#include <mutex>
#include <vector>
#include <chrono>


const double claw_to_center_dist{250}; //to be confirmed

enum Status{
    IDLE,
    BUOYS_SEARCHING,BUOYS_SEARCHING_EDGE,BUOYS_SEARCHING_STRAY,
    BUOYS_FOUND,
    BUOYS_ROTATING,BUOYS_APPROACHING,BUOYS_GRABBING,BUOYS_LIFTING,
    BUOYS_COLLECT_DONE,BUOYS_REVERSING,BUOYS_DONE_DECIDING,

    BUOYS_FIND_FAIRWAY,BUOYS_TO_FAIRWAY,BUOYS_NEAR_FAIRWAY,
    BUOYS_FAIRWAY_CLAW_UP,BUOYS_FAIRWAY_CLAW_GRABBING,
    BUOYS_FAIRWAY_HOLDER_OPENING,BUOYS_FAIRWAY_CLAW_DOWN,
    BUOYS_FAIRWAY_CLAW_RELEASING,BUOYS_FAIRWAY_REVERSE_DECIDING,
    BUOYS_FAIRWAY_REVERSING,BUOYS_FAIRWAY_DONE_DECIDING,

    LIGHTHOUSE_FINDING,LIGHTHOUSE_GOING_TO,LIGHTHOUSE_ROTATING,
    LIGHTHOUSE_APPROACHING,LIGHTHOUSE_PRESSED,LIGHTHOUSE_REVERSING,
    LIGHTHOUSE_DONE_DECIDING,

    WINDSOCKS_FINDING,WINDSOCKS_GOING_TO,WINDSOCKS_ROTATING,
    WINDSOCKS_APPROACHING,WINDSOCKS_ERECTING,WINDSOCKS_REVERSING,
    WINDSOCKS_DONE_DECIDING,

    PARKING_FINDING_WEATHERVANE,PARKING_TO_WEATHERVANE,PARKING_WAITING_FOR_SCAN,
    PARKING_GOING_TO_BAY,PARKING_DONE,

    DEADLINE_REACHED,PROBLEM_OCCURRED
};

std::ostream& operator<<(std::ostream& s,Status x);

enum StatFlags{
    REVOLVER_ROTATING=1,
    CLAW_LIFTING=2,
    CLAW_ROTATING=4,
    CLAW_CLAMPING=8,
    //CLAW_PENDING=16,
    //REVOLVER_HOLDER_PENDING=32,
    BUOYS_EDGE=64,
    WINDSOCK_1_DONE=128,
    WINDSOCK_2_DONE=256,
    LIGHTHOUSE_DONE=512,
    ARRIVED=1024,
    FAIRWAY_MULTI=2048
};

std::ostream& operator<<(std::ostream& s,StatFlags sf);

class System{
    std::chrono::steady_clock::time_point timeInit;
    long int time; //time in ms since start of match
    long int lastSignalTime; //time since last i2c signal to motor board
    long int eventTimer; //for use by update()
    long int deadline; //if reaches deadline, store status, go to DEADLINE_REACHED

    int points; //estimation of points
    int southPark; //0=unknown,1=south,2=north
    FairwayTypes currentFairway;
    Position tmpCupPosition;

    Status status;
    Status tmpStat; //to store status while temporary action is performed
    int flags;

    std::vector<ScheduledEvent> events;
    std::mutex events_mutex;

    Route route;
    Robot robot;
    Arena arena;
public:
    System();

    void startTime();
    void updateTime();

    void update(); //checks scheduled events

    void addEvent(ScheduledEvent e); //needs lock
    long int getTime(){return time;};
    Robot& getRobot(){return robot;};
};

#endif
