#include "route.h"
#include "system.h"
#include <cmath>

const double pi = std::atan(1)*4;

const double min_edge_dist = 50;
const double angle_increment = 0.4;
const double large_dist_length = 10000;
const double angle_jump = 1;
const double distance_jump = 15;
const int max_recursion_count = 30;

const double slow_down_dist = 150;
const double tolerance_radius = 30;
const double small_tol_rad = 7;
const double slow_speed = 0.5;
const double backtrack_dist = 80;

const double max_route_deviation = 50;
const float rotate_speed = 50;
const double angle_rotate_limit = 20;
const double rotation_coefficient = 0.4; //for each mm off target, value to decrease PWM by

const long min_motor_signal_period = 50;

Route::Route(Position current,Position target,const Bitmap* map){
	approachAngle = -2;
	setRoute(current,target,map);
}
Route::Route(Position current,Position target,double targetAng,const Bitmap* map){
	approachAngle = targetAng;
	setRoute(current,target,targetAng,map);
}

double getDistFromAngle(Position p,double ang,const Bitmap* map){
    auto pt = map->closestSetBit(Line(p,ang,large_dist_length));
    if(pt)return getDistance(p,*pt);
    else return 100000000;
}

bool Route::setRoute(Position current,Position target,double targetAng,const Bitmap* map){
	approachAngle = targetAng;
	if(approachAngle < -1.0) return setRoute(current,target,map);
	//y-sin,x-cos
	Position altTarget(target.x-backtrack_dist*std::cos(targetAng*pi/180),
		target.y-backtrack_dist*std::sin(targetAng*pi/180));
	if(!setRoute(current,altTarget,map))return 0;
	RoutePoint r(target);
	r.tolRegion = Circle(small_tol_rad,target);
	r.motorSpeed = slow_speed;
	points.push_back(r);
	return 1;
}

bool Route::setRoute(Position current,Position target,const Bitmap* map){
    std::cout<<"Setting route\n";
	points.clear();
	points.push_back(current);
	if(!findRoute(current,target,map)){
        points.clear();
        return 0;
    }
	double distFromTarget = 0;
	bool afterSlow = true; //at the point in the route after slowing down
	for(int i=points.size()-1;i>0;--i){
		if(afterSlow){
			points[i].motorSpeed = slow_speed;
			points[i].tolRegion = Circle(small_tol_rad,points[i].point);

			Line path(points[i-1].point,points[i].point);
			double d = slow_down_dist - distFromTarget;
			if(path.getLength() > d){
				//find point to start slowing down
				RoutePoint newPt(path.getPoint(d));
				newPt.tolRegion = Circle(small_tol_rad,points[i].point);
				points.insert(points.begin()+i,newPt); //insert before i
				afterSlow = false;
			}
		}else{
			points[i].tolRegion = Circle(tolerance_radius,points[i].point);
		}
	}
	index = 1;
	lastDist = large_dist_length;
	return 1;
}

int Route::findRoute(Position start,Position end,const Bitmap* map,int rCount){
    //std::cout<<"Finding route between "<<start<<" and "<<end<<"\n";
    Rectangle boundries(Point(0,3000),Point(2000,0));
    if(!boundries.contains(start) || !boundries.contains(end)){
        //std::cout<<"Out of bounds\n";
        return 0;
    }

    //Test for clear route
    if(map->lineClear(Line(start,end))){
        //std::cout<<"Line clear\n";
        points.push_back(RoutePoint(end));
        return 1;
    }
    //Check rCount
    if(rCount > max_recursion_count){
        //std::cout<<"Recursion limit reached\n";
        return -2; //point index to revert back to
    }
    //Otherwise find 'bump'
    double angle=Line(start,end).getAngle();
    //std::cout<<"Line angle: "<<angle<<"\n";
    double pDistPrev,pDist,nDistPrev,nDist;
    std::optional<Position> pt = map->closestSetBit(Line(start,end));
    if(pt)
        pDistPrev=pDist=nDistPrev=nDist = getDistance(start,*pt);
    for(double angDist=angle_increment;angDist<180;angDist+=angle_increment){
        pDist = getDistFromAngle(start,angle+angDist,map);
        nDist = getDistFromAngle(start,angle-angDist,map);
        //std::cout<<"Angle dist: "<<angDist<<" pDist,nDist: "<<pDist<<","<<nDist<<"\n";
        bool foundEdge=0;
        Position newStart(0,0);
        if(pDist-pDistPrev>min_edge_dist){
            //std::cout<<"Anticlockwise forward jump\n";
            newStart = getPoint(start,angle+angDist+angle_jump,pDistPrev+distance_jump);
            foundEdge = true;
        }else if(pDistPrev-pDist>min_edge_dist){
            //std::cout<<"Anticlockwise backward jump\n";
            newStart = getPoint(start,angle+angDist-angle_jump,pDist+distance_jump);
            foundEdge = true;
        }
        if(nDist-nDistPrev>min_edge_dist){
            //std::cout<<"Clockwise forward jump\n";
            newStart = getPoint(start,angle-angDist-angle_jump,pDistPrev+distance_jump);
            foundEdge = true;
        }else if(nDistPrev-nDist>min_edge_dist){
            //std::cout<<"Clockwise backward jump\n";
            newStart = getPoint(start,angle-angDist+angle_jump,pDist+distance_jump);
            foundEdge = true;
        }
        if(foundEdge){
            //std::cout<<"New start point: "<<newStart<<"\n";
            points.push_back(RoutePoint(newStart));
            int res = findRoute(newStart,end,map,rCount+1);
            if(res==1)return 1;
            else if(res<=0) points.pop_back();
            if(res<0 && -res!=rCount) return res;
        }
        pDistPrev = pDist;
        nDistPrev = nDist;
    }
    index = 1;
    return 0;
}

bool Route::update(System* sys,const Bitmap* map){
    Position p = sys->getRobot().coordinates;
    double orientation = sys->getRobot().orientation;

    RoutePoint target = points[index];
    //index 0 is start, so it should have already got there
    if(index == 0) ++index;
    //if reached point
    if(target.tolRegion.contains(p)){
        std::cout<<"Reached checkpoint\n";
        if(index == points.size()-1){
            std::cout<<"Reached end of route\n";
            return 1;
        }
        ++index;
    }
    //check deviation and clearness of path
    Line path(points[index].point,points[index-1].point);
    if(getDistToLine(p,path)>max_route_deviation || !map->lineClear(path)){
        std::cout<<"Off route! Rerouting\n";
        std::cout<<"Distance to line segment ("<<path<<") of "<<p<<" is ";
        std::cout<<getDistToLine(p,path)<<"\nMap path clear? "<<map->lineClear(path)<<"\n";
        if(!setRoute(p,points.back().point,approachAngle,map)) throw NO_VIABLE_ROUTE;
    }
	//test if gone past point, skip point or reroute
    double currDist = getDistance(p,target.point);
    if(currDist > lastDist+1){
        std::cout<<"Gone past point\n";
        Line npath(points[index].point,points[index+1].point);
        if(map->lineClear(npath)) ++index;
        else if(!setRoute(p,points.back().point,approachAngle,map)) throw NO_VIABLE_ROUTE;
    }

	//update values
	target = points[index];
	currDist = getDistance(p,target.point);

	if(sys->getTime()-lastSignalTime > min_motor_signal_period){
        double angleOff = orientation - getAngle(p,target.point);
        //probably after arriving at a point
        if(angleOff > angle_rotate_limit){
            sys->getRobot().mechanisms.setDrive(rotate_speed,-rotate_speed); //clockwise
        }else if(angleOff < -angle_rotate_limit){
            sys->getRobot().mechanisms.setDrive(-rotate_speed,rotate_speed); //anticlockwise
        }else{
            double distOff = std::abs(currDist * std::tan(pi/180*angleOff));
            double handicap = distOff*rotation_coefficient;
            handicap = handicap <= 100 ? handicap : 100; //make sure it's <= 100

            if(angleOff >= 0){
                sys->getRobot().mechanisms.setDrive(100*target.motorSpeed,(100 - handicap)*target.motorSpeed);//clockwise
            }else{
                sys->getRobot().mechanisms.setDrive((100 - handicap)*target.motorSpeed,100*target.motorSpeed);//anticlockwise
            }
        }
        lastSignalTime = sys->getTime();
    }

    lastDist = currDist;
    return false;
}

void Route::print()const{
    for(auto rp:points){
        std::cout<<"Point "<<rp.point<<", speed "<<rp.motorSpeed<<"\n";
    }
    for(int x=0;x<30;++x){
        for(int y=0;y<45;++y){
            bool n=false;
            for(int k=0;k<points.size();++k){
                if(static_cast<int>(points[k].point.x/2000*30)==x &&
                        static_cast<int>(points[k].point.y/3000*100)==y){
                    std::cout<<k%10;
                    n=true;
                    break;
                }
            }
            if(!n)std::cout<<"-";
        }
        std::cout<<"\n";
    }
}
void Route::print(const Bitmap& map)const{
    for(auto rp:points){
        std::cout<<"Point "<<rp.point<<", speed "<<rp.motorSpeed<<"\n";
    }
    int k=0;
    for(int x=0;x<30;++x){
        for(int y=0;y<100;++y){
            bool n=false;
            for(int k=0;k<points.size();++k){
                if(static_cast<int>(points[k].point.x/2000*30)==x &&
                        static_cast<int>(points[k].point.y/3000*100)==y){
                    std::cout<<k%10;
                    n=true;
                    break;
                }
            }
            if(!n){
                if(map.getBit(Position(x*2000/30,y*3000/100)))std::cout<<"+";
                else std::cout<<"-";
            }
        }
        std::cout<<"\n";
    }
}
















//
