#include "arenaobject.h"

int ArenaObject::nextId = 0;


ArenaObject::ArenaObject():id(nextId++),center(0,0),isRed(false){};

ArenaObject::ArenaObject(Polygon r,Position c,double ang,bool red):isRed(red),
    dangerRegion(r),center(c),orientation(ang),id(nextId++){};

void ArenaObject::translateTo(Position p){
    dangerRegion.translate(p.x-center.x,p.y-center.y);
    center = p;
}


std::ostream& operator<<(std::ostream& s,const ArenaObject& ao){
    return s<<"Position: "<<ao.center<<" ID: "<<ao.id<<" Red? "<<ao.isRed;
}
