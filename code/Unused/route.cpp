
const double min_edge_dist = 50;
const double angle_increment = 0.5;
const double large_dist_length = 10000;
const double angle_jump = 1;
const double distance_jump = 15;
const int max_recursion_count = 30;
const double max_route_deviation = 50;

inline double getDistFromAngle(Position p,double ang,Bitmap* map){
    return getDistance(p,map.closestSetBit(Line(p,ang,large_dist_length)));
}

int Route::findRoute(Position start,Position end,Bitmap* map,int rCount=0){
    //Test for clear route
    if(map->lineClear(Line(start,end))){
        points.push_back(RoutePoint(end));
        return 1;
    }
    //Check rCount
    if(rCount > max_recursion_count) return -2; //point index to revert back to
    //Otherwise find 'bump'
    double angle=Line(start,end).getAngle()
    double pDistPrev,pDist,nDistPrev,nDist;
    pDistPrev=pDist=nDistNeg=nDist = getDistance(start,map->closestSetBit(Line(start,end)));
    for(double angDist=angle_increment;angDist<180;angDist+=angle_increment){
        pDist = getDistFromAngle(start,angle+angDist,map);
        nDist = getDistFromAngle(start,angle-angDist,map);

        bool foundEdge=0;
        Position newStart;
        if(pDist-pDistPrev>min_edge_dist){
            newStart = getPoint(start,angle+angDist+angle_jump,pDistPrev+distance_jump);
            foundEdge = true;
        }else if(pDistPrev-pDist>min_edge_dist){
            newStart = getPoint(start,angle+angDist-angle_jump,pDist+distance_jump);
            foundEdge = true;
        }
        if(nDist-nDistPrev>min_edge_dist){
            newStart = getPoint(start,angle-angDist-angle_jump,pDistPrev+distance_jump);
            foundEdge = true;
        }else if(nDistPrev-nDist>min_edge_dist){
            newStart = getPoint(start,angle-angDist+angle_jump,pDist+distance_jump);
            foundEdge = true;
        }

        if(foundEdge){
            points.push_back(RoutePoint(newStart));
            int res = findRoute(newStart,end,map,rCount+1);
            if(res==1)return 1;
            else if(res==0) points.pop_back();
            if(res<0 && -res!=rCount) return res;
        }
        pDistPrev = pDist;
        nDistPrev = nDist;
    }
    index = 1;
    return 0;
}

bool Route::update(Mechanisms* mech,Position p,double orientation,Bitmap* map){
    RoutePoint target = points[index];
    //index 0 is start, so it should have already got there
    if(index == 0) ++index;
    //if reached point
    if(target.tolRegion.contains(p)){
        if(index == points.size()-1) return 1;
        ++index;
    }
    //check deviation and clearness of path
    Line path(points[index].point,points[index-1].point);
    if(getDistToLine(p,path)>max_route_deviation || !map.lineClear(path)){
        if(!findRoute(p,points.last().point,map)) throw NO_VIABLE_ROUTE;
    }
    double currDist = getDistance(p,target);
    if(currDist > lastDist){
        Line npath(points[index].point,points[index+1].point);
        if(map.lineClear(npath)) ++index;
        else if(!findRoute(p,points.last().point,map)) throw NO_VIABLE_ROUTE;
    }



    target = points[index];
}


















//
