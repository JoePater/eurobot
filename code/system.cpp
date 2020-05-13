#include "system.h"
#include "robot.h"

#include <chrono>
#include <iostream>
#include <bitset>
#include <unistd.h>

#define DEBUG_MODE

const double claw_to_center_dist = 250; //to be confirmed

const int buoys_found_deadline = 12000;
const int buoys_rotation_deadline = 5000;
const int buoys_approaching_deadline = 5000;
const int buoys_reversing_deadline = 3000;

const int buoys_to_fairway_deadline = 10000;
const int buoys_near_fairway_deadline = 5000;
const int buoys_fairway_reversing_deadline = 5000;

const int claw_up_deadline = 2000;
const int claw_grabbing_deadline = 1500;
const int claw_down_deadline = 2000;
const int claw_release_deadline = 1500;
const int claw_down_edge_deadline = 3000;
const int claw_up_edge_deadline = 3500;

const int lighthouse_going_to_deadline = 10000;
const int lighthouse_rotating_deadline = 4000;
const int lighthouse_approaching_deadline = 5000;
const int lighthouse_reversing_deadline = 4000;

const int windsocks_going_to_deadline = 10000;
const int windsocks_rotating_deadline = 4000;
const int windsocks_approaching_deadline = 4000;
const int windsocks_erecting_deadline = 4000;
const int windsocks_reversing_deadline = 4000;

const int parking_to_weathervane_deadline = 8000;
const int parking_waiting_for_scan_deadline = 3000;
const int parking_going_to_bay_deadline = 8000;

const int holder_clamp_time = 200;
const int lighthouse_press_time = 500;
const int windsocks_erect_time = 1000;

const double buoy_edge_approach_dist = 250; //distance from cup
const double buoy_stray_approach_dist = 250;
const double buoy_edge_reverse_dist = 250; //dist from cup
const double fairway_incremental_dist = 80; //dist to reverse
const double fairway_reverse_dist = 100; //dist to reverse
const double lighthouse_approach_dist = 200; //dist from lighthouse to press
const double lighthouse_reverse_dist = 250; //dist from edge
const double windsocks_edge_dist = 120;
const double windsocks_reverse_dist = 180; //dist from edge

const long int windsock_stop_time = 20000;
const long int lighthouse_stop_time = 35000;
const long int buoy_stop_time = 80000;
const int buoy_enough_time = 22000;
const int fairway_enough_time = 15000;

const float slow_approach_speed = 20;
const float slow_rotation_speed = 20;

const int min_motor_signal_period = 50;

std::ostream& operator<<(std::ostream& s,Status x){
    switch(x){
        case IDLE: return s<<"idle";
        case BUOYS_SEARCHING: return s<<"buoys searching";
        case BUOYS_SEARCHING_EDGE: return s<<"buoys searching edge";
        case BUOYS_SEARCHING_STRAY: return s<<"buoys searching stray";
        case BUOYS_FOUND: return s<<"buoys found";
        case BUOYS_ROTATING: return s<<"buoys rotating";
        case BUOYS_APPROACHING: return s<<"buoys approaching";
        case BUOYS_GRABBING: return s<<"buoys grabbing";
        case BUOYS_LIFTING: return s<<"buoys lifting";
        case BUOYS_COLLECT_DONE: return s<<"buoys collect done";
        case BUOYS_REVERSING: return s<<"buoys reversing";
        case BUOYS_DONE_DECIDING: return s<<"buoys done deciding";
        case BUOYS_FIND_FAIRWAY: return s<<"buoys find fairway";
        case BUOYS_TO_FAIRWAY: return s<<"buoys to fairway";
        case BUOYS_NEAR_FAIRWAY: return s<<"buoys near fairway";
        case BUOYS_FAIRWAY_CLAW_UP: return s<<"buoys fairway claw up";
        case BUOYS_FAIRWAY_CLAW_GRABBING: return s<<"buoys fairway claw grabbing";
        case BUOYS_FAIRWAY_HOLDER_OPENING: return s<<"buoys fairway holder opening";
        case BUOYS_FAIRWAY_CLAW_DOWN: return s<<"buoys fairway claw down";
        case BUOYS_FAIRWAY_CLAW_RELEASING: return s<<"buoys fairway claw releasing";
        case BUOYS_FAIRWAY_REVERSING: return s<<"buoys fairway reversing";
        case BUOYS_FAIRWAY_DONE_DECIDING: return s<<"buoys fairway done deciding";
        case LIGHTHOUSE_FINDING: return s<<"lighthouse finding";
        case LIGHTHOUSE_GOING_TO: return s<<"lighthouse going to";
        case LIGHTHOUSE_ROTATING: return s<<"lighthouse rotating";
        case LIGHTHOUSE_APPROACHING: return s<<"lighthouse approaching";
        case LIGHTHOUSE_PRESSED: return s<<"lighthouse pressed";
        case LIGHTHOUSE_REVERSING: return s<<"lighthouse reversing";
        case LIGHTHOUSE_DONE_DECIDING: return s<<"lighthouse done deciding";
        case WINDSOCKS_FINDING: return s<<"windsocks finding";
        case WINDSOCKS_GOING_TO: return s<<"windsocks going to";
        case WINDSOCKS_ROTATING: return s<<"windsocks rotating";
        case WINDSOCKS_APPROACHING: return s<<"windsocks approaching";
        case WINDSOCKS_ERECTING: return s<<"windsocks erecting";
        case WINDSOCKS_REVERSING: return s<<"windsocks reversing";
        case WINDSOCKS_DONE_DECIDING: return s<<"windsocks done deciding";
        case PARKING_FINDING_WEATHERVANE: return s<<"parking finding weathervane";
        case PARKING_TO_WEATHERVANE: return s<<"parking to weathervane";
        case PARKING_WAITING_FOR_SCAN: return s<<"parking waiting for scan";
        case PARKING_GOING_TO_BAY: return s<<"parking going to bay";
        case PARKING_DONE: return s<<"parking done";
        case DEADLINE_REACHED: return s<<"deadline reached";
        case PROBLEM_OCCURRED: return s<<"problem occurred";
    }
    return s<<"error";
}
std::ostream& operator<<(std::ostream& s,StatFlags sf){
     return s<<std::bitset<32>(sf);
}

System::System():deadline(-1),points(0),southPark(0),status(IDLE),
    tmpStat(IDLE),tmpCupPosition(0,0),flags(0){
    startTime();
    addEvent(ScheduledEvent([this](){return getTime()>95500;},
        [this](){getRobot().mechanisms.erectFlag();}));
    addEvent(ScheduledEvent([this](){return getTime()>100000;},
        [this](){getRobot().mechanisms.setRoutine(STOP_ALL);}));
}

void System::startTime(){
    timeInit = std::chrono::steady_clock::now();
}

void System::updateTime(){
#ifndef DEBUG_MODE
    auto t = std::chrono::steady_clock::now();
    auto dt=std::chrono::duration_cast<std::chrono::duration<double>>(t-timeInit);
    time = static_cast<long int>(dt.count()*1000);
#else
    std::cout<<"Enter time: ";
    std::cin>>time;
#endif
}

void System::update(){
    //update time,check for ScheduledEvents
    updateTime();

    std::unique_lock<std::mutex> lock(events_mutex);
    for(auto ev=events.begin();ev<events.end();++ev){
        if(*ev){
            (*ev)();
            events.erase(ev);
        }
    }
    lock.unlock();

    //check for deadline
    if(time > deadline && deadline != -1){
        tmpStat = status;
        status = DEADLINE_REACHED;
    }

#ifdef DEBUG_MODE
    std::cout<<"\n\nCurrent time: "<<time<<" Current deadline: "<<deadline;
    std::cout<<" Status: "<<status<<"\nFlags: "<<std::bitset<16>(flags);
    std::cout<<"\nOrientation: "<<robot.orientation<<" Position: "<<robot.coordinates;
    std::cout<<"\nM,A to see mech, arena, P,R to set position,";
    std::cout<<"rotation, O to set opponent position\n>>>";
    std::string s;
    std::cin>>s;
    if(std::string::npos!=s.find('M'))std::cout<<"Mechanisms:\n"<<robot.mechanisms<<"\n";
    if(std::string::npos!=s.find('A')){
        std::cout<<"Fairways\n"<<arena.getFairways()<<"Arena:\n";
        arena.getMap().print();
    }if(std::string::npos!=s.find('P')){
        std::cout<<"Enter x: ";std::cin>>robot.coordinates.x;
        std::cout<<"Enter y: ";std::cin>>robot.coordinates.y;
    }if(std::string::npos!=s.find('R')){
        std::cout<<"Enter rotation: ";std::cin>>robot.orientation;
    }if(std::string::npos!=s.find('O')){
        std::cout<<"Enter opponent X: ";
        std::cin>>arena.getOpponent().x;
        std::cout<<"Enter opponent Y: ";
        std::cin>>arena.getOpponent().y;
    }
#endif // DEBUG_MODE
    switch(status){
    case IDLE:{
        //decide what to do next
        if(time < windsock_stop_time &&
            (!(flags&WINDSOCK_1_DONE)|!(flags&WINDSOCK_2_DONE))){
            status = WINDSOCKS_FINDING;
        }else if(time < lighthouse_stop_time && !(flags&LIGHTHOUSE_DONE)){
            status = LIGHTHOUSE_FINDING;
        }else{
            status = PARKING_FINDING_WEATHERVANE;
        }
        break;
    }case BUOYS_SEARCHING:{
        //find closest cup, set route, set mechanisms
        Position cupEdge(0,0);
		Position cupStray(0,0);
		Position edgeTmp(0,0);
		Position strayTmp(0,0);
		//find viable cup options
        int i=0;
        do{
			cupEdge=arena.getClosestEdgeBuoy(robot.coordinates,i).getPosition();
			edgeTmp = cupEdge;
			//get proper coordinates x away, should invalidate cup
			if(cupEdge.x < 0)cupEdge.x += buoy_edge_approach_dist;
			else if(cupEdge.y < 0)cupEdge.y += buoy_edge_approach_dist;
			else if(cupEdge.y > 3000)cupEdge.y -= buoy_edge_approach_dist;
			if(++i>4){break;}
		}while(!route.setRoute(robot.coordinates,cupEdge,&arena.getMap()));
		i=0;
		do{
			cupStray=arena.getClosestStrayBuoy(robot.coordinates,i).getPosition();
			strayTmp = cupStray;
			//get proper coordinates x away
			cupStray = Line(robot.coordinates,cupStray).getPoint(buoy_stray_approach_dist);
			if(++i>4){break;}
		}while(!route.setRoute(robot.coordinates,cupStray,&arena.getMap()));

        //compare distances
		double distEdge = getDistance(cupEdge,robot.coordinates);
		double distStray = getDistance(cupStray,robot.coordinates);
		if(distStray < distEdge*1.1){
			//route already set, set mechanisms
			tmpCupPosition = strayTmp;
			robot.mechanisms.setRoutine(CLAW_DOWN);
			flags = flags&~BUOYS_EDGE;
			flags |= CLAW_LIFTING;
		}else{
            tmpCupPosition = edgeTmp;
			route.setRoute(robot.coordinates,cupEdge,&arena.getMap());
			robot.mechanisms.setRoutine(CLAW_DOWN_EDGE);
			flags |= BUOYS_EDGE;
			flags |= CLAW_LIFTING;
		}
        status = BUOYS_FOUND;
        deadline = time + buoys_found_deadline;
        break;
    }case BUOYS_FOUND:{
        //update flags, follow route, then start rotating
		if(time-lastSignalTime > min_motor_signal_period){
            int revProg = robot.mechanisms.getRevProgress();
			int prog = robot.mechanisms.getClawProgress();
			//if not rotating
			if(revProg==0){
                flags &= ~REVOLVER_ROTATING;
			}
			//if currently lowering claw
			if(flags & CLAW_LIFTING){
                //if fully lowered
				if(prog==0){
                    //change flags, start releasing claw
					flags &= ~CLAW_LIFTING;
					flags |= CLAW_CLAMPING;
					robot.mechanisms.setRoutine(RELEASE);

					//go to right holder slot
					int rotation = robot.mechanisms.getRevolver().nextFree();
					robot.mechanisms.setRevolverRotation(rotation);
					flags |= REVOLVER_ROTATING;
				}
			}else if(flags & CLAW_CLAMPING){
				if(prog==0){
					flags &= ~CLAW_CLAMPING;
				}
			}
		}
		//if not arrived yet
        if(!(flags & ARRIVED)){
            //if arrived
            if(route.update(this,&arena.getMap())){
                flags |= ARRIVED;
            }
        }else{
            //already arrived, waiting on other flags
            if(!(flags & (CLAW_LIFTING | CLAW_CLAMPING | REVOLVER_ROTATING))){
                status = BUOYS_ROTATING;
                flags &= ~ARRIVED;

                //start rotating
                double angleOff=getAngle(route.getTarget(),robot.coordinates)-robot.orientation;
                if(angleOff > 5){robot.mechanisms.setDrive(-slow_rotation_speed,slow_rotation_speed);}
                else if(angleOff < -5){robot.mechanisms.setDrive(slow_rotation_speed,-slow_rotation_speed);}
            }
        }
        break;
    }case BUOYS_ROTATING:{
        //determine when to stop rotating
        double angleOff=getAngle(tmpCupPosition,robot.coordinates)-robot.orientation;
        if(std::abs(angleOff) < 5){
            robot.mechanisms.setDrive(slow_approach_speed,slow_approach_speed);
            status = BUOYS_APPROACHING;
            deadline = time + buoys_approaching_deadline;
        }
        break;
    }case BUOYS_APPROACHING:{
        //wait for colour detector, then set clamping
		Buoy b = robot.mechanisms.getCupColor();
		if(b!=EMPTY){
			robot.mechanisms.setRoutine(CLAMP,b);
			status = BUOYS_GRABBING;
			deadline = time + claw_grabbing_deadline;
		}
        break;
    }case BUOYS_GRABBING:{
        //wait for grabbing to finish, set lifting
		if(time-lastSignalTime > min_motor_signal_period){
			//grabbing finished
			if(robot.mechanisms.getClawProgress() == 0){
                arena.removeBuoy(flags & BUOYS_EDGE ?
                    arena.getClosestEdgeBuoy(tmpCupPosition,0).getId():
                    arena.getClosestStrayBuoy(tmpCupPosition,0).getId());
				robot.mechanisms.setRoutine(flags & BUOYS_EDGE ? CLAW_UP_EDGE : CLAW_UP);
				status = BUOYS_LIFTING;
				deadline = time + claw_up_deadline;
			}
			lastSignalTime = time;
		}
        break;
    }case BUOYS_LIFTING:{
        //wait for lifting to finish, set holder pending
		if(time-lastSignalTime > min_motor_signal_period){
			//lifting finished
			if(robot.mechanisms.getClawProgress() == 0){
				eventTimer = time;
				//not properly reached top
				if(!robot.mechanisms.cupHolderPressed(-1)){
					robot.mechanisms.setCupHolderClosed(-1);
				}
				eventTimer = time;
				status = BUOYS_COLLECT_DONE;
				deadline = -1;
			}
			lastSignalTime = time;
		}
        break;

    }case BUOYS_COLLECT_DONE:{
        //wait for holder time to finish, start reversing if edge
		if(time-eventTimer > holder_clamp_time){
            //release claw
            robot.mechanisms.setRoutine(RELEASE);
            flags |= CLAW_CLAMPING;
            status = BUOYS_REVERSING;
            deadline = time + buoys_reversing_deadline;
			if(flags & BUOYS_EDGE){
				robot.mechanisms.setDrive(-slow_approach_speed,-slow_approach_speed); //reverse
			}else{
				flags |= ARRIVED;
			}
		}
        break;
    }case BUOYS_REVERSING:{
        //wait for position to be far enough away if edge
		if(getDistance(robot.coordinates,tmpCupPosition) > buoy_edge_reverse_dist
                && !(flags & ARRIVED)){
			robot.mechanisms.setDrive(0,0);
			flags |= ARRIVED;
		}
		if((flags & ARRIVED)&& !(flags & CLAW_CLAMPING)){
            flags &= ~ARRIVED;
            status = BUOYS_DONE_DECIDING;
			deadline = -1;
		}
		if(time-lastSignalTime > min_motor_signal_period){
            if(flags & CLAW_CLAMPING){
                if(robot.mechanisms.getClawProgress()==0){
                    flags &= ~CLAW_CLAMPING;
                }
            }
            lastSignalTime = time;
		}
        break;
    }case BUOYS_DONE_DECIDING:{
        //decide what to do next
        long int timeLeft = buoy_stop_time-time;
        //enough time and revolver not full
        if(timeLeft > buoy_enough_time && !robot.mechanisms.getRevolver().isFull()){
            status = BUOYS_SEARCHING;
        }else if(timeLeft > fairway_enough_time){
            status = BUOYS_FIND_FAIRWAY;
        }else{
            status = PARKING_FINDING_WEATHERVANE;
        }
        break;

    }case BUOYS_FIND_FAIRWAY:{
        //choose fairway, then set route (and mechanisms)
        Holder h = robot.mechanisms.getRevolver().currentBuoy();
        if(h!=HOLDER_GREEN && h!=HOLDER_RED){
            if(!robot.mechanisms.getRevolver().isEmpty()){
                int path = robot.mechanisms.getRevolver().nextBuoy();
                robot.mechanisms.setRevolverRotation(path);
                flags |= REVOLVER_ROTATING;
                h = robot.mechanisms.getRevolver().currentBuoy();
            }else{
                status = IDLE;
                deadline = -1;
                break;
            }
        }
        currentFairway = arena.getFairways().getBestFairway(robot.coordinates,h==HOLDER_RED);
        Position p = arena.getFairways().getFairwayPosition(currentFairway);
        if(!route.setRoute(robot.coordinates,p,&arena.getMap())){
            tmpStat = status;
            status = PROBLEM_OCCURRED;
        }else{
            tmpCupPosition = arena.getFairways().getCupPosition(currentFairway);
            robot.mechanisms.setRoutine(RELEASE);
            flags |= CLAW_CLAMPING;
            status = BUOYS_TO_FAIRWAY;
            deadline = time + buoys_to_fairway_deadline;
        }
        break;
    }case BUOYS_TO_FAIRWAY:{
        //follow route, prepare claw
        if(time-lastSignalTime > min_motor_signal_period){
            int prog = robot.mechanisms.getClawProgress();
            int revProg = robot.mechanisms.getRevProgress();
            if(flags & REVOLVER_ROTATING){
                if(revProg==0){
                    flags &= ~REVOLVER_ROTATING;
                }
            }
            if(flags & CLAW_CLAMPING){
                if(prog==0){
                    flags &= ~CLAW_CLAMPING;
                    flags |= CLAW_LIFTING;
                    robot.mechanisms.setRoutine(CLAW_UP_EDGE);
                }
            }else if(flags & CLAW_LIFTING){
                if(prog==0){
                    flags &= ~CLAW_LIFTING;
                }
            }
            lastSignalTime = time;
        }

        if(flags & ARRIVED){
            if(!(flags & (CLAW_LIFTING | CLAW_CLAMPING | REVOLVER_ROTATING))){
                flags &= ~ARRIVED;
                status = BUOYS_NEAR_FAIRWAY;
                deadline = time + buoys_near_fairway_deadline;
            }
        }else if(route.update(this,&arena.getMap())){
            flags |= ARRIVED;
        }
        break;
    }case BUOYS_NEAR_FAIRWAY:{
        //update using fairways object, then set claw grabbing
        if(arena.getFairways().updateDrive(
                this,robot.coordinates,robot.orientation,currentFairway)){
            status = BUOYS_FAIRWAY_CLAW_GRABBING;
            deadline = time + claw_grabbing_deadline;
            robot.mechanisms.setRoutine(CLAMP);
        }
        break;
    }case BUOYS_FAIRWAY_CLAW_GRABBING:{
        //wait for claw to finish grabbing, set holder opening
        if(time-lastSignalTime > min_motor_signal_period){
            if(robot.mechanisms.getClawProgress()==0){
                robot.mechanisms.setCupHolderOpen(-1);
                status = BUOYS_FAIRWAY_HOLDER_OPENING;
                eventTimer = time + holder_clamp_time;
                deadline = -1;
            }
            lastSignalTime = time;
        }
        break;
    }case BUOYS_FAIRWAY_HOLDER_OPENING:{
        //wait for time period, set claw lowering
        if(time > eventTimer){
            robot.mechanisms.setRoutine(CLAW_DOWN);
            status = BUOYS_FAIRWAY_CLAW_DOWN;
            deadline = time + claw_down_deadline;
        }
        break;
    }case BUOYS_FAIRWAY_CLAW_DOWN:{
        //wait for claw to reach bottom, set claw releasing
        if(time-lastSignalTime > min_motor_signal_period){
            if(robot.mechanisms.getClawProgress()==0){
                robot.mechanisms.setRoutine(RELEASE);
                status = BUOYS_FAIRWAY_CLAW_RELEASING;
                deadline = time + claw_release_deadline;
            }
            lastSignalTime = time;
        }
        break;
    }case BUOYS_FAIRWAY_CLAW_RELEASING:{
        //wait for claw to release, set reversing
        if(time-lastSignalTime > min_motor_signal_period){
            if(robot.mechanisms.getClawProgress()==0){
                arena.getFairways().addBuoys(currentFairway);
                status = BUOYS_FAIRWAY_REVERSE_DECIDING;
                deadline = -1;
            }
            lastSignalTime = time;
        }
        break;
    }case BUOYS_FAIRWAY_REVERSE_DECIDING:{
        //consider sequential placements
        int cnt=0;
        int rot=0;
        if(currentFairway==BIG_GREEN || currentFairway==SMALL_GREEN){
            cnt = robot.mechanisms.getRevolver().totalGreen();
            rot = robot.mechanisms.getRevolver().nextGreen();
        }else if(currentFairway==BIG_RED || currentFairway==SMALL_RED){
            cnt = robot.mechanisms.getRevolver().totalRed();
            rot = robot.mechanisms.getRevolver().nextRed();
        }
        if(cnt > 0 && arena.getFairways().isFree(currentFairway)){
            robot.mechanisms.setRevolverRotation(rot);
            flags |= (REVOLVER_ROTATING | FAIRWAY_MULTI);
        }
        robot.mechanisms.setDrive(-slow_approach_speed,-slow_approach_speed);
        status = BUOYS_FAIRWAY_REVERSING;
        deadline = time + buoys_fairway_reversing_deadline;
        break;
    }case BUOYS_FAIRWAY_REVERSING:{
        //wait for sufficient distance, set deciding
        //consider sequential placements
        if(flags & FAIRWAY_MULTI){
            if(time-lastSignalTime > min_motor_signal_period){
                if(flags & REVOLVER_ROTATING){
                    if(robot.mechanisms.getRevProgress()==0){
                        flags &= ~REVOLVER_ROTATING;
                        flags |= CLAW_LIFTING;
                        robot.mechanisms.setRoutine(CLAW_UP);
                    }
                }else if(flags & CLAW_LIFTING){
                    if(robot.mechanisms.getClawProgress()==0){
                        flags &= ~CLAW_LIFTING;
                    }
                }
            }
        }
        if(flags & ARRIVED){
            if(flags & FAIRWAY_MULTI){
                if(!(flags & (REVOLVER_ROTATING|CLAW_LIFTING))){
                    robot.mechanisms.setRoutine(CLAMP);
                    flags &= ~ARRIVED;
                    status = BUOYS_FAIRWAY_CLAW_GRABBING;
                    deadline = time + claw_grabbing_deadline;
                }
            }else{
                flags &= ~ARRIVED;
                status = BUOYS_FAIRWAY_DONE_DECIDING;
                deadline = -1;
            }
        }else{
            double d = getDistance(robot.coordinates,tmpCupPosition);
            double targetDist = flags & FAIRWAY_MULTI ? fairway_incremental_dist : fairway_reverse_dist;
            if(d > targetDist){
                robot.mechanisms.setDrive(0,0);
                flags |= ARRIVED;
            }
        }
        break;
    }case BUOYS_FAIRWAY_DONE_DECIDING:{
        //decide what to do next
        if(!robot.mechanisms.getRevolver().isEmpty()){
            status = BUOYS_FIND_FAIRWAY;
        }else{
            status = IDLE;
        }
        break;

    }case LIGHTHOUSE_FINDING:{
        //set route to lighthouse
        Position p = arena.getLighthousePos();
        if(route.setRoute(robot.coordinates,p,&arena.getMap())){
            robot.mechanisms.setRoutine(CLAW_DOWN_EDGE);
            flags |= CLAW_LIFTING;
            status = LIGHTHOUSE_GOING_TO;
            deadline = time + lighthouse_going_to_deadline;
        }else{
            tmpStat = status;
            status = PROBLEM_OCCURRED;
        }
        break;
    }case LIGHTHOUSE_GOING_TO:{
        //follow route, start rotating
        if(flags & CLAW_LIFTING){
            if(time-lastSignalTime > min_motor_signal_period){
                if(robot.mechanisms.getClawProgress()==0){
                    flags &= ~CLAW_LIFTING;
                }
                lastSignalTime = time;
            }
        }
        if(route.update(this,&arena.getMap())){
            flags |= ARRIVED;
        }
        if(flags & ARRIVED){
            if(!(flags & CLAW_LIFTING)){
                flags &= ~ARRIVED;
                if(robot.orientation < 175){
                    robot.mechanisms.setDrive(-slow_rotation_speed,slow_rotation_speed);
                }else if(robot.orientation > 185){
                    robot.mechanisms.setDrive(slow_rotation_speed,-slow_rotation_speed);
                }
                status = LIGHTHOUSE_ROTATING;
                deadline = time + lighthouse_rotating_deadline;
            }
        }
        break;
    }case LIGHTHOUSE_ROTATING:{
        //wait for correct orientation, start driving
        if(robot.orientation > 175 && robot.orientation < 185){
            robot.mechanisms.setDrive(slow_approach_speed,slow_approach_speed);
            status = LIGHTHOUSE_APPROACHING;
            deadline = time + lighthouse_approaching_deadline;
        }
        break;
    }case LIGHTHOUSE_APPROACHING:{
        //wait for correct position, start timing
        if(robot.coordinates.x < lighthouse_approach_dist){
            eventTimer = time;
            status = LIGHTHOUSE_PRESSED;
            deadline = -1;
        }
        break;
    }case LIGHTHOUSE_PRESSED:{
        //wait for 500ms, start reversing
        if(time-eventTimer > lighthouse_press_time){
            robot.mechanisms.setDrive(-slow_approach_speed,-slow_approach_speed);
            flags |= LIGHTHOUSE_DONE;
            status = LIGHTHOUSE_REVERSING;
            deadline = time + lighthouse_reversing_deadline;
        }
        break;
    }case LIGHTHOUSE_REVERSING:{
        //wait for correct position, stop, start deciding
        if(robot.coordinates.x > lighthouse_reverse_dist){
            robot.mechanisms.setDrive(0,0);
            status = LIGHTHOUSE_DONE_DECIDING;
            deadline = -1;
        }
        break;
    }case LIGHTHOUSE_DONE_DECIDING:{
        //decide what to do next
        status = IDLE;
        break;

    }case WINDSOCKS_FINDING:{
        //decide which windsock to go to, set route + claw
        Position p(0,0);
        if(!(flags & WINDSOCK_1_DONE)){
            p = arena.getWindsock(1);
        }else if(!(flags & WINDSOCK_2_DONE)){
            p = arena.getWindsock(2);
        }else{
            status = WINDSOCKS_DONE_DECIDING;
            break;
        }
        #ifdef DEBUG_MODE
        std::cout<<"Windsocks finding position: "<<p<<"\n";
        #endif
        if(route.setRoute(robot.coordinates,p,&arena.getMap())){
            robot.mechanisms.setRoutine(CLAW_DOWN_EDGE);
            flags |= CLAW_LIFTING;
            status = WINDSOCKS_GOING_TO;
            deadline = time + windsocks_going_to_deadline;
        }else{
            tmpStat = status;
            status = PROBLEM_OCCURRED;
        }

        break;
    }case WINDSOCKS_GOING_TO:{
        //follow route, then start rotating
        if(route.update(this,&arena.getMap())){
            #ifdef DEBUG_MODE
            std::cout<<"Windsocks arrived\n";
            #endif
            flags |= ARRIVED;
        }
        if(flags & ARRIVED && !(flags & (CLAW_LIFTING|CLAW_CLAMPING))){
            if(robot.orientation < 355 && robot.orientation > 180){
                #ifdef DEBUG_MODE
                std::cout<<"Windsocks rotating anticlockwise\n";
                #endif
                robot.mechanisms.setDrive(-slow_rotation_speed,slow_rotation_speed);
            }else if(robot.orientation > 5 && robot.orientation < 180){
                #ifdef DEBUG_MODE
                std::cout<<"Windsocks rotating clockwise\n";
                #endif
                robot.mechanisms.setDrive(slow_rotation_speed,-slow_rotation_speed);
            }
            flags &= ~ARRIVED;
            status = WINDSOCKS_ROTATING;
            deadline = time + windsocks_rotating_deadline;
        }
        if(time-lastSignalTime > min_motor_signal_period && (flags & (CLAW_LIFTING|CLAW_CLAMPING))){
            int prog = robot.mechanisms.getClawProgress();
            if(flags & CLAW_LIFTING && prog==0){
                flags &= ~CLAW_LIFTING;
                robot.mechanisms.setRoutine(CLAMP);
                flags |= CLAW_CLAMPING;
            }else if(flags & CLAW_CLAMPING && prog==0){
                flags &= ~CLAW_CLAMPING;
            }
        }
        break;
    }case WINDSOCKS_ROTATING:{
        //wait until orientation correct, start approach
        if(robot.orientation > 355 || robot.orientation < 5){
            #ifdef DEBUG_MODE
            std::cout<<"Windsocks finished rotating, starting to approach\n";
            #endif
            robot.mechanisms.setDrive(slow_approach_speed,slow_approach_speed);
            status = WINDSOCKS_APPROACHING;
            deadline = windsocks_approaching_deadline;
        }
        break;
    }case WINDSOCKS_APPROACHING:{
        //wait until position correct, start rotating
        if(robot.coordinates.x > 2000-windsocks_edge_dist){
            #ifdef DEBUG_MODE
            std::cout<<"Windsocks reached destination\n";
            #endif
            eventTimer = time;
            if(team == BLUE){
                robot.mechanisms.setDrive(-slow_rotation_speed,slow_rotation_speed);
            }else{
                robot.mechanisms.setDrive(slow_rotation_speed,-slow_rotation_speed);
            }
            status = WINDSOCKS_ERECTING;
            deadline = time + windsocks_erecting_deadline;
        }
        break;
    }case WINDSOCKS_ERECTING:{
        //wait until time/orientation correct, start reversing
        if(time - eventTimer > windsocks_erect_time){
            robot.mechanisms.setDrive(-slow_approach_speed,-slow_approach_speed);
            status = WINDSOCKS_REVERSING;
            deadline = time + windsocks_reversing_deadline;
        }
        break;
    }case WINDSOCKS_REVERSING:{
        //wait until position correct, stop
        if(flags & CLAW_CLAMPING){
            if(time-lastSignalTime > min_motor_signal_period){
                if(robot.mechanisms.getClawProgress()==0){
                    flags &= ~CLAW_CLAMPING;
                }
            }
        }
        if(!(flags & CLAW_CLAMPING)){
            if(robot.coordinates.x < 2000-windsocks_reverse_dist){
                flags |= ARRIVED;
            }
        }
        if(flags & ARRIVED && !(flags & CLAW_CLAMPING)){
            robot.mechanisms.setDrive(0,0);
            robot.mechanisms.setRoutine(RELEASE);
            status = WINDSOCKS_DONE_DECIDING;
            flags &= ~ARRIVED;
            deadline = -1;
        }
        break;
    }case WINDSOCKS_DONE_DECIDING:{
        //decide what to do next
        status = WINDSOCKS_FINDING;
        break;

    }case PARKING_FINDING_WEATHERVANE:{
        //set route to good view of weathervane
        if(!southPark){
            Position p = arena.getWeathervane();
            if(route.setRoute(robot.coordinates,p,&arena.getMap())){
                status = PARKING_TO_WEATHERVANE;
                deadline = time + parking_to_weathervane_deadline;
            }else{
                tmpStat = status;
                status = PROBLEM_OCCURRED;
            }
        }else{
            Position p(0,0);
            if(southPark==1){
                p = arena.getSouthBay();
            }else{
                p = arena.getNorthBay();
            }
            if(route.setRoute(robot.coordinates,p,&arena.getMap())){
                status = PARKING_GOING_TO_BAY;
                deadline = time + parking_going_to_bay_deadline;
            }else{
                tmpStat = status;
                status = PROBLEM_OCCURRED;
            }
        }
        break;
    }case PARKING_TO_WEATHERVANE:{
        //follow route to weathervane
        if(route.update(this,&arena.getMap())){
            robot.mechanisms.setDrive(0,0);
            status = PARKING_WAITING_FOR_SCAN;
            deadline = time + parking_waiting_for_scan_deadline;
        }
        break;
    }case PARKING_WAITING_FOR_SCAN:{
        //wait for known parking bay, set route
        if(southPark > 0){
            Position p(0,0);
            if(southPark==1){
                p = arena.getSouthBay();
            }else{
                p = arena.getNorthBay();
            }
            if(route.setRoute(robot.coordinates,p,&arena.getMap())){
                status = PARKING_GOING_TO_BAY;
                deadline = time + parking_going_to_bay_deadline;
            }else{
                tmpStat = status;
                status = PROBLEM_OCCURRED;
            }
        }
        break;
    }case PARKING_GOING_TO_BAY:{
        //follow route, stop
        if(route.update(this,&arena.getMap())){
            robot.mechanisms.setDrive(0,0);
            status = PARKING_DONE;
            deadline = -1;
        }
        break;
    }case PARKING_DONE:{
        //show score, stop everything
        robot.mechanisms.displayNumber(points);
        robot.mechanisms.setRoutine(STOP_ALL);
        break;

    }case DEADLINE_REACHED:{
        robot.mechanisms.setRoutine(STOP_ALL);
        status = IDLE;
        break;

    }case PROBLEM_OCCURRED:{
        robot.mechanisms.setRoutine(STOP_ALL);
        status = IDLE;
        break;
    }
    }
}

void System::addEvent(ScheduledEvent e){
    std::unique_lock<std::mutex> lock(events_mutex);
    events.push_back(e);
}
