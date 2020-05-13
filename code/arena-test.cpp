
#include "fairways.h"
#include "arena.h"
#include "route.h"
#include <chrono>



int main(int argc, char** argv){
    team = BLUE;
    Arena arena;
    arena.updateBitmap();
    std::cout<<"Map:\n";
    arena.getMap().print();
    Route r;
    Position p1(1000,300),p2(1000,2000);
    std::cout<<"Setting route between "<<p1<<" and "<<p2<<":\n";
    if(!r.setRoute(p1,p2,&arena.getMap()))std::cout<<"Failed\n";
    r.print(arena.getMap());

    p1 = Position(100,200);
    p2 = Position(1800,2000);
    std::cout<<"Setting route between "<<p1<<" and "<<p2<<":\n";
    if(!r.setRoute(p1,p2,&arena.getMap()))std::cout<<"Failed\n";
    r.print(arena.getMap());

    p1 = Position(1980,1200);
    p2 = Position(1200,100);
    std::cout<<"Setting route between "<<p1<<" and "<<p2<<":\n";
    if(!r.setRoute(p1,p2,&arena.getMap()))std::cout<<"Failed\n";
    r.print(arena.getMap());
    return 0;
}
