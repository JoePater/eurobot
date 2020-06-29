#ifndef ARENA_H
#define ARENA_H

#include <vector>
#include <map>
#include "geometry.h"
#include "arenaobject.h"
#include "fairways.h"
#include "bitmap.h"

enum ObjectType{
    GREEN_BUOY,RED_BUOY,WINDSOCK_1,WINDSOCK_2,LIGHTHOUSE,WEATHERVANE,
    NORTH_ZONE,SOUTH_ZONE,OPPONENT
};

class Arena{
    std::vector<ArenaObject> strayBuoys;
    std::vector<ArenaObject> edgeBuoys;
    ArenaObject lighthouse;
    std::pair<ArenaObject,ArenaObject> windsocks;
    ArenaObject northZone;
    ArenaObject southZone;
    ArenaObject weathervane;
    Fairways fairways;

    ArenaObject opponent;

    Bitmap safeMap;
public:
    Arena();
    Arena(int rows,int cols);
    const Bitmap& getMap()const{return safeMap;};
    //bool updateObject(ObjectType type,Position p,bool canCreate=false); //true if already exists
    ArenaObject getBuoy(int id);
    void removeBuoy(int id);
    void updateBitmap();
    ArenaObject getClosestStrayBuoy(Position p,int i=0);//ith closest buoy
    ArenaObject getClosestEdgeBuoy(Position p,int i=0);
    Fairways& getFairways(){return fairways;};
    Position getLighthousePos(){return lighthouse.getPosition();};
    Position getWindsock(int n){return n==2 ? windsocks.second.getPosition() : windsocks.first.getPosition();};
    Position getWeathervane(){return weathervane.getPosition();};
    Position getSouthBay(){return southZone.getPosition();};
    Position getNorthBay(){return northZone.getPosition();};
    Position& getOpponent(){return opponent.getPosition();};
    void updateOppnent(Position p);
};









#endif
