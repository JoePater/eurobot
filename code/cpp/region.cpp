#include "region.h"
#include <cmath>

constexpr const double pi = std::atan(1)*4;
const Position large_negative_position(-10001,-10000);

std::ostream& operator<<(std::ostream& s,const Polygon& p){
    s<<"(";
    for(int i=0;i<p.vertices.size()-1;++i){
        s<<p.vertices[i]<<",";
    }
    return s<<p.vertices.back()<<")";
}

void Polygon::clearVertices(){vertices.clear();}
void Polygon::addVertex(const Position& p){vertices.push_back(p);}
void Polygon::removeVertex(int index){
    if(index < 0 || index >= vertices.size())return;
    vertices.erase(vertices.begin()+index);
}
int Polygon::vertexCount()const{return vertices.size();}
Position Polygon::getVertex(int index)const{
    if(index < 0 || index >= vertices.size())throw 1;
    return vertices[index];
}
bool Polygon::overlaps(const Polygon& r)const{
    for(int i=r.vertexCount()-1;i>=0;--i){
        if(contains(r.getVertex(i)))return true;
    }
    for(int i=vertexCount()-1;i>=0;--i){
        if(r.contains(getVertex(i)))return true;
    }
    return false;
}
void Polygon::translate(double x,double y){
    for(int i=0;i<vertices.size();++i){
        vertices[i].x = vertices[i].x + x;
        vertices[i].y = vertices[i].y + y;
    }
}
bool Polygon::contains(const Position& p)const{
    Line l(large_negative_position,p);
    //std::cout<<"Long\n";
    //std::cout<<" P: "<<p<<" L: "<<l<<" ";
    int numIntersections = 0;
    for(int a=0;a<vertices.size();++a){
        int b = a+1;
        if(b == vertices.size()) b = 0;
        Line segment(vertices[a],vertices[b]);
        if(intersects(segment,l)){
            //std::cout<<"Line "<<l<<" intersects segment "<<segment<<"\n";
            ++numIntersections;
        }else{
            //std::cout<<"Line "<<l<<" doesn't intersect segment "<<segment<<"\n";
        }
    }
    return (numIntersections % 2 == 1);
}
bool Polygon::isEmpty()const{
    return (vertices.size()<=2);
}
Position Polygon::closestPoint(const Position& p)const{
    if(vertices.empty())throw 1;
    Position closest = vertices[0];
    double dist = getDistance(closest,p);
    for(int a=0;a<vertices.size();++a){
        int b = a+1;
        if(b == vertices.size()) b = 0;
        Line segment(vertices[a],vertices[b]);
        Position x = getLineClosestPoint(p,segment);
        if(getDistance(x,p)<dist){
            closest = x;
            dist = getDistance(x,p);
        }
    }
    return closest;
}
Position Polygon::getTopLeft()const{
    if(vertices.empty())throw 1;
    Position tl = vertices[0];
    for(Position v:vertices){
        if(v.x < tl.x)tl.x = v.x;
        if(v.y > tl.y)tl.y = v.y;
    }
    return tl;
}
Position Polygon::getBottomRight()const{
    if(vertices.empty())throw 1;
    Position br = vertices[0];
    for(Position v:vertices){
        if(v.x > br.x)br.x = v.x;
        if(v.y < br.y)br.y = v.y;
    }
    return br;
}


Rectangle::Rectangle(){
    vertices.push_back(Position(0,1));
    vertices.push_back(Position(1,1));
    vertices.push_back(Position(1,0));
    vertices.push_back(Position(0,0));
}
Rectangle::Rectangle(const Position& topLeft,const Position& bottomRight){
    vertices.push_back(topLeft);
    vertices.push_back(Position(bottomRight.x,topLeft.y));
    vertices.push_back(bottomRight);
    vertices.push_back(Position(topLeft.x,bottomRight.y));
}
void Rectangle::setTopLeft(const Position& p){
    vertices[0] = p;
    vertices[1] = Position(vertices[2].x,p.y);
    vertices[3] = Position(p.x,vertices[2].y);
}
void Rectangle::setBottomRight(const Position& p){
    vertices[2] = p;
    vertices[1] = Position(p.x,vertices[0].y);
    vertices[3] = Position(vertices[0].x,p.y);
}


Circle::Circle():rad(1),center(0,0){
    updateVertices();
}
Circle::Circle(double r,const Position& c):rad(r),center(c){
    updateVertices();
}
void Circle::setRad(double r){
    rad = r;
    updateVertices();
}
void Circle::setCenter(const Position& c){
    center = c;
    updateVertices();
}
void Circle::setParameters(double r,const Position& c){
    rad = r;
    center = c;
    updateVertices();
}
void Circle::updateVertices(int n){
    if(n <= 1)return;
    vertices.clear();
    for(double ang=0;ang<=2*pi-0.0001;ang+=2*pi/n){
        Position p(rad*std::cos(ang),rad*std::sin(ang));
        p = p+center;
        vertices.push_back(p);
    }
}
bool Circle::contains(const Position& p)const{
    Point tmp = p-center;
    return std::hypot(tmp.x,tmp.y) < rad;
}







//
