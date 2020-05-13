#include "arena.h"
#include <algorithm>

Arena::Arena():Arena(2000,3000){};

Arena::Arena(int rows,int cols):safeMap(rows,cols),
    lighthouse(Polygon(),team==BLUE?Position(200,250):Position(200,2750),0),
    northZone(Polygon(),team==BLUE?Position(300,250):Position(300,2750),0),
    southZone(Polygon(),team==BLUE?Position(1300,250):Position(1300,2750),0),
    weathervane(Polygon(),team==BLUE?Position(500,1300):Position(500,1700),0),
    opponent(team==BLUE?Circle(200,Point(800,2800)):Circle(200,Point(800,200)),
        team==BLUE?Position(800,2800):Position(800,200),0),
    windsocks(ArenaObject(Polygon(),team==BLUE?Position(1800,220):Position(1800,1780),0),
        ArenaObject(Polygon(),team==BLUE?Position(1800,625):Position(1800,1375),0))
{
    //set up positions for all arena objects known in advance
    if(team==BLUE){
        strayBuoys.push_back(ArenaObject(Circle(200,Position(400,300)),Position(400,300),0));
        strayBuoys.push_back(ArenaObject(Circle(200,Position(510,450)),Position(510,450),0));
        strayBuoys.push_back(ArenaObject(Circle(200,Position(100,670)),Position(100,670),0));
        strayBuoys.push_back(ArenaObject(Circle(200,Position(400,950)),Position(400,950),0));
        strayBuoys.push_back(ArenaObject(Circle(200,Position(800,1100)),Position(800,1100),0));
        strayBuoys.push_back(ArenaObject(Circle(200,Position(1200,1270)),Position(1200,1270),0));
        strayBuoys.push_back(ArenaObject(Circle(200,Position(1080,450)),Position(1080,450),0));
        strayBuoys.push_back(ArenaObject(Circle(200,Position(1200,300)),Position(1200,300),0));
        strayBuoys.push_back(ArenaObject(Circle(200,Position(1955,1005)),Position(1955,1005),0));
        strayBuoys.push_back(ArenaObject(Circle(200,Position(1955,1395)),Position(1955,1395),0));
        strayBuoys.push_back(ArenaObject(Circle(200,Position(1650,1065)),Position(1650,1065),0));
        strayBuoys.push_back(ArenaObject(Circle(200,Position(1650,1335)),Position(1650,1335),0));
        edgeBuoys.push_back(ArenaObject(Circle(200,Position(1450,-67)),Position(1450,-67),0));
        edgeBuoys.push_back(ArenaObject(Circle(200,Position(1525,-67)),Position(1525,-67),0));
        edgeBuoys.push_back(ArenaObject(Circle(200,Position(1600,-67)),Position(1600,-67),0));
        edgeBuoys.push_back(ArenaObject(Circle(200,Position(1675,-67)),Position(1675,-67),0));
        edgeBuoys.push_back(ArenaObject(Circle(200,Position(1750,-67)),Position(1750,-67),0));
        edgeBuoys.push_back(ArenaObject(Circle(200,Position(-67,700)),Position(-67,700),0));
        edgeBuoys.push_back(ArenaObject(Circle(200,Position(-67,775)),Position(-67,775),0));
        edgeBuoys.push_back(ArenaObject(Circle(200,Position(-67,850)),Position(-67,850),0));
        edgeBuoys.push_back(ArenaObject(Circle(200,Position(-67,925)),Position(-67,925),0));
        edgeBuoys.push_back(ArenaObject(Circle(200,Position(-67,1000)),Position(-67,1000),0));
    }else{
        strayBuoys.push_back(ArenaObject(Circle(200,Position(400,2700)),Position(400,2700),0));
        strayBuoys.push_back(ArenaObject(Circle(200,Position(510,2550)),Position(510,2550),0));
        strayBuoys.push_back(ArenaObject(Circle(200,Position(100,2330)),Position(100,2330),0));
        strayBuoys.push_back(ArenaObject(Circle(200,Position(400,1050)),Position(400,1050),0));
        strayBuoys.push_back(ArenaObject(Circle(200,Position(800,1900)),Position(800,1900),0));
        strayBuoys.push_back(ArenaObject(Circle(200,Position(1200,1730)),Position(1200,1730),0));
        strayBuoys.push_back(ArenaObject(Circle(200,Position(1080,1550)),Position(1080,1550),0));
        strayBuoys.push_back(ArenaObject(Circle(200,Position(1200,2700)),Position(1200,2700),0));
        strayBuoys.push_back(ArenaObject(Circle(200,Position(1955,1995)),Position(1955,1995),0));
        strayBuoys.push_back(ArenaObject(Circle(200,Position(1955,1605)),Position(1955,1605),0));
        strayBuoys.push_back(ArenaObject(Circle(200,Position(1650,1935)),Position(1650,1935),0));
        strayBuoys.push_back(ArenaObject(Circle(200,Position(1650,1665)),Position(1650,1665),0));
        edgeBuoys.push_back(ArenaObject(Circle(200,Position(1450,3067)),Position(1450,3067),0));
        edgeBuoys.push_back(ArenaObject(Circle(200,Position(1525,3067)),Position(1525,3067),0));
        edgeBuoys.push_back(ArenaObject(Circle(200,Position(1600,3067)),Position(1600,3067),0));
        edgeBuoys.push_back(ArenaObject(Circle(200,Position(1675,3067)),Position(1675,3067),0));
        edgeBuoys.push_back(ArenaObject(Circle(200,Position(1750,3067)),Position(1750,3067),0));
        edgeBuoys.push_back(ArenaObject(Circle(200,Position(-67,1300)),Position(-67,1300),0));
        edgeBuoys.push_back(ArenaObject(Circle(200,Position(-67,2225)),Position(-67,2225),0));
        edgeBuoys.push_back(ArenaObject(Circle(200,Position(-67,2150)),Position(-67,2150),0));
        edgeBuoys.push_back(ArenaObject(Circle(200,Position(-67,2075)),Position(-67,2075),0));
        edgeBuoys.push_back(ArenaObject(Circle(200,Position(-67,2000)),Position(-67,2000),0));
    }
    updateBitmap();
}

//use std::optional<>?
ArenaObject Arena::getBuoy(int id){
    for(auto b:strayBuoys){
        std::cout<<"ID: "<<b.getId()<<"\n";
        if(b.getId()==id) return b;
    }
    for(auto b:edgeBuoys){
    std::cout<<"ID: "<<b.getId()<<"\n";
        if(b.getId()==id) return b;
    }
    throw 1;
}

void Arena::removeBuoy(int id){
    Position p(-500,-500);
    for(auto b=strayBuoys.begin();b<strayBuoys.end();++b){
        if(b->getId()==id){
            p = b->getPosition();
            strayBuoys.erase(b);
            break;
        }
    }
    for(auto b=edgeBuoys.begin();b<edgeBuoys.end();++b){
        if(b->getId()==id){
            p = b->getPosition();
            strayBuoys.erase(b);
            break;
        }
    }
    Circle c=Circle(202,p);
    safeMap.clearRegion(&c);
    for(auto b=strayBuoys.begin();b<strayBuoys.end();++b){
        if(getDistance(b->getPosition(),p)<202){
            safeMap.setRegion(&b->getRegion());
            std::cout<<"A\n";
        }
    }
    for(auto b=edgeBuoys.begin();b<edgeBuoys.end();++b){
        if(getDistance(b->getPosition(),p)<202){
            safeMap.setRegion(&b->getRegion());
            std::cout<<"A\n";
        }
    }
}

void Arena::updateBitmap(){
    safeMap.clearAll();
    for(auto b:strayBuoys){
        safeMap.setRegion(&b.getRegion());
    }
    for(auto b:edgeBuoys){
        safeMap.setRegion(&b.getRegion());
    }
    safeMap.setRegion(&opponent.getRegion());
}

ArenaObject Arena::getClosestStrayBuoy(Position p,int i){
    std::vector<ArenaObject> sorted = strayBuoys;
    std::sort(sorted.begin(),sorted.end(),[p](ArenaObject a,ArenaObject b)
        {return getDistance(p,a.getPosition())<getDistance(p,b.getPosition());});
    return sorted[i];
}

ArenaObject Arena::getClosestEdgeBuoy(Position p,int i){
    std::vector<ArenaObject> sorted = edgeBuoys;
    std::sort(sorted.begin(),sorted.end(),[p](ArenaObject a,ArenaObject b)
        {return getDistance(p,a.getPosition())<getDistance(p,b.getPosition());});
    return sorted[i];
}

void Arena::updateOppnent(Position p){
    opponent.translateTo(p);

}
























//
