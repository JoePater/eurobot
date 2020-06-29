#include "geometry.h"
#include <cmath>

const double pi = 3.1415927;

std::ostream& operator<<(std::ostream& s,const Point& p){
    return s<<"("<<p.x<<","<<p.y<<")";
}
double getDistance(Point a,Point b){
    return std::hypot(a.x-b.x,a.y-b.y);
}
double getGradient(Point a,Point b){
    return (b.y-a.y)/(b.x-a.x);
}
double getAngle(Point a,Point b){
    double ang = std::atan((b.y-a.y)/(b.x-a.x))/pi*180;
    if(a.x > b.x) ang *= -1;
    while(ang<0)ang+=360;
    while(ang>=360)ang-=360;
    return ang;
}
Point getPoint(Point start,double angle,double dist){
    Point d(dist*std::cos(angle*pi/180),dist*std::sin(angle*pi/180));
    return start+d;
}
Point Point::operator-(Point b)const{
    return Point(x-b.x,y-b.y);
}
Point Point::operator+(Point b)const{
    return Point(x+b.x,y+b.y);
}
Point operator*(Point a,double x){
    return Point(a.x*x,a.y*x);
}
Point operator*(double x,Point a){
    return Point(a.x*x,a.y*x);
}
Point operator/(Point a,double x){
    return Point(a.x/x,a.y/x);
}

Line::Line(Point s,Point e):start(s),end(e){};
Line::Line(double sx,double sy,double ex,double ey):
    start(Point(sx,sy)),end(Point(ex,ey)){}; //constructs from x and y of start and end
Line::Line(Point s,double angle,double length):
    start(s),end(::getPoint(s,angle,length)){};
double Line::getLength()const{
    return getDistance(start,end);
}
Point Line::getPoint(double dist,bool fromEnd)const{
    double w=dist/getLength();
    if(fromEnd){
        return start*w+end*(1-w);
    }else{
        return start*(1-w)+end*w;
    }
} //get point dist from end (or start)
std::ostream& operator<<(std::ostream& s,const Line& l){
    return s<<l.start<<","<<l.end;
}
int orientation(Point p, Point q, Point r)
{
    int val = (q.y-p.y) * (r.x-q.x) - (q.x-p.x) * (r.y-q.y);
    if (val == 0) return 0;  // colinear
    return (val > 0)? 1: 2; // clock or counterclock wise
}

bool intersects(const Line& l1,const Line& l2){
    Point p1=l1.getStart();
    Point q1=l1.getEnd();
    Point p2=l2.getStart();
    Point q2=l2.getEnd();
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    return ((o1 != o2) && (o3 != o4));
}
Point getLineClosestPoint(Point p,const Line& l){
    Line l2(l.getStart(),p);
    Line l3(l.getEnd(),p);
    Position d(0,0);
    d.x = l2.getLength()*std::cos((l2.getAngle()-l.getAngle())*pi/180)*
        std::cos(l.getAngle()*pi/180);
    d.y = l2.getLength()*std::cos((l2.getAngle()-l.getAngle())*pi/180)*
            std::sin(l.getAngle()*pi/180);

    double a = std::abs(l2.getAngle()-l.getAngle());
    while(a<0)a+=360;
    while(a>=360)a-=360;
    if(a > 90){return l.getStart();}

    a = std::abs(l.getAngle()+180-l3.getAngle());
    while(a<0)a+=360;
    while(a>=360)a-=360;
    if(a > 90){return l.getEnd();}

    return l.getStart()+d;
}
double getDistToLine(Point p,const Line& l){
    return getDistance(getLineClosestPoint(p,l),p);
}









//
