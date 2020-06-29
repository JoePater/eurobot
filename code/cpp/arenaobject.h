#ifndef ARENAOBJECT_H
#define ARENAOBJECT_H

/*
To do:
 - Done
*/

#include "geometry.h"
#include "region.h"

class ArenaObject{
    int id;

    Polygon dangerRegion;
    Position center;
    double orientation;

    bool isRed; //used for buoys
public:
    static int nextId;

    ArenaObject();
    ArenaObject(Polygon r,Position c,double ang,bool red=false);
    //ArenaObject& operator=(ArenaObject&&)=default;
    //ArenaObject(ArenaObject&&)=default;

    Position getConstPosition()const{return center;};
    Position& getPosition(){return center;};
    const Polygon& getRegion()const{return dangerRegion;};
    double getOrientation()const{return orientation;};
    int getId()const{return id;};
    bool getIsRed()const{return isRed;};

    void translateTo(Position p);

    void setPos(Position p){center=p;};
    void setReg(const Polygon& reg){dangerRegion=reg;};
    void setOrientation(double x){orientation=x;};

    friend std::ostream& operator<<(std::ostream& s,const ArenaObject& ao);
};









#endif
