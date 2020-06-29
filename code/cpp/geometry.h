#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <iostream>


class Point{
public:
    double x,y;
    Point(double x0,double y0):x(x0),y(y0){};
    Point operator-(Point a)const;
    Point operator+(Point a)const;
};
std::ostream& operator<<(std::ostream& s,const Point& p);

double getDistance(Point a,Point b);
double getGradient(Point a,Point b);
double getAngle(Point a,Point b);

Point getPoint(Point start,double angle,double dist);
Point operator*(Point a,double x);
Point operator*(double x,Point a);
Point operator/(Point a,double x);

typedef Point Position;

class Line{
private:
    Point start;
    Point end;
public:
    Line()=default;
    Line(Point s,Point e);
    Line(double sx,double sy,double ex,double ey); //constructs from x and y of start and end
    Line(Point s,double angle,double length);
    double getLength()const;
    Point getPoint(double dist,bool fromEnd=true)const; //get point dist from end (or start)
    Point getStart()const{return start;};
    Point getEnd()const{return end;};
    double getGradient()const{return ::getGradient(start,end);};
    double getAngle()const{return ::getAngle(start,end);};
    friend std::ostream& operator<<(std::ostream& s,const Line& l);
};
std::ostream& operator<<(std::ostream& s,const Line& l);
bool intersects(const Line& l1,const Line& l2);
Point getLineClosestPoint(Point p,const Line& l);
double getDistToLine(Point p,const Line& l);


#endif
