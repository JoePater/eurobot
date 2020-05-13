#ifndef REGION_H
#define REGION_H

#include <vector>
#include <iostream>
#include "geometry.h"

/*class Region{
public:
    Region() = default;
    virtual bool contains(const Position& p)=0; //draw line from edge to p, count how many of edges intersect
    virtual bool isEmpty()=0;
    virtual Position closestPoint(const Position& p)=0;
    virtual Position getTopLeft()=0;
    virtual Position getBottomRight()=0;
};*/
class Polygon{
protected:
    std::vector<Position> vertices;
public:
    void clearVertices();
    void addVertex(const Position& p);
    void removeVertex(int index);
    int vertexCount()const;
    Position getVertex(int index)const;
    bool overlaps(const Polygon& r)const;
    void translate(double x,double y);
    virtual bool contains(const Position& p)const;
    virtual bool isEmpty()const;
    virtual Position closestPoint(const Position& p)const;
    virtual Position getTopLeft()const;
    virtual Position getBottomRight()const;
    friend std::ostream& operator<<(std::ostream& s,const Polygon& p);
};
std::ostream& operator<<(std::ostream& s,const Polygon& p);
//vertices anticlockwise starting with top left
class Rectangle:public Polygon{
public:
    Rectangle();
    Rectangle(const Position& topLeft,const Position& bottomRight);
    void setTopLeft(const Position& x);
    void setBottomRight(const Position& x);
};
class Circle:public Polygon{
    double rad;
    Position center;
    void updateVertices(int n=10);
public:
    Circle();
    Circle(double r,const Position& c); //add lots of vertices
    void setRad(double r); //check whether needs to move vertices or create them
    void setCenter(const Position& c);
    void setParameters(double r,const Position& c);
    virtual bool contains(const Position& p)const;
};
#endif
