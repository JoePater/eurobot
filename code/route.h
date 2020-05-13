#ifndef ROUTE_H
#define ROUTE_H

#include <vector>
#include "bitmap.h"
#include "geometry.h"
#include "region.h"

class System;

class RoutePoint{
public:
    Position point;
    Polygon tolRegion;
    double motorSpeed; //between 0 and 1, scaling factor
    RoutePoint(Position p):point(p),motorSpeed(1){};
};

class Route{
    //index 0 = start
    double lastDist;
    int index;
    double approachAngle; // <-1 if not applicable
    std::vector<RoutePoint> points;
    long int lastSignalTime;
public:
    Route()=default;
    Route(Position current,Position target,const Bitmap* map);
    Route(Position current,Position target,double targetAng,const Bitmap* map);

	bool setRoute(Position current,Position target,const Bitmap* map);
	bool setRoute(Position current,Position target,double targetAng,const Bitmap* map);

    int findRoute(Position start,Position end,const Bitmap* map,int rCount=0);

    bool update(System* sys,const Bitmap* map);
    //bool update(System* sys,Position p,double orientation,const Bitmap* map); //1 if arrived

    Position getTarget()const{return points.back().point;};
    void print()const;
    void print(const Bitmap& map)const;
};







#endif
