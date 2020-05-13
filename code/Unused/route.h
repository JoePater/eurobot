#ifndef ROUTE_H
#define ROUTE_H

/*
To do:
 - Write route finding algorithm
 - Write constructor
 - Write update()

*/

class RoutePoint{
    Position point;
    Region tolRegion;
    double motorSpeed; //between 0 and 1, scaling factor
    RoutePoint(Position p):point(p){};
};

class Route{
    //index 0 = start
    double lastDist;
    int index;
    std::vector<RoutePoint> points;
public:
    Route()=default;
    Route(Position current,Position target,Bitmap* map);
    Route(Position current,Position target,double targetAng,Bitmap* map)
    //Route(const Robot& robot,Position target,Bitmap* map);
    //Route(const Robot& robot,Position target,double targetAng,Bitmap* map)
    int findRoute(Position start,Position end,Bitmap* map);
    bool update(Mechanisms* mech,Position p,double orientation,Bitmap* map); //1 if arrived
};







#endif
