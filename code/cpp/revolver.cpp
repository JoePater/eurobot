#include "revolver.h"
#include <cmath>

const int twist_limit = 5;

Revolver::Revolver():current(0),holders(5,HOLDER_OPEN){}

int Revolver::clockwiseLimit()const{return twist_limit-current;}
int Revolver::anticlockwiseLimit()const{return -twist_limit+current;}

Holder Revolver::currentBuoy()const{
    return holders[current % 5];
}
bool Revolver::isFull()const{
    for(Holder b:holders){
        if(b==HOLDER_OPEN)return false;
    }
    return true;
}
int Revolver::nextFree()const{
    int shortestPath = 100;
    for(int i=anticlockwiseLimit();i<=clockwiseLimit();++i){
        if(std::abs(i)<std::abs(shortestPath)&&
            (holders[(current+i)%5]==HOLDER_OPEN || holders[(current+i)%5]==HOLDER_WAITING))
            shortestPath = i;
    }
    return shortestPath;
}
int Revolver::nextRed()const{
    int shortestPath = 100;
    for(int i=anticlockwiseLimit();i<=clockwiseLimit();++i){
        if(std::abs(i)<std::abs(shortestPath)&&
            (holders[(current+i)%5]==HOLDER_RED))
            shortestPath = i;
    }
    return shortestPath;
}
int Revolver::nextGreen()const{
    int shortestPath = 100;
    for(int i=anticlockwiseLimit();i<=clockwiseLimit();++i){
        if(std::abs(i)<std::abs(shortestPath)&&
            (holders[(current+i)%5]==HOLDER_GREEN))
            shortestPath = i;
    }
    return shortestPath;
}
int Revolver::nextBuoy()const{
    int shortestPath = 100;
    for(int i=anticlockwiseLimit();i<=clockwiseLimit();++i){
        if(std::abs(i)<std::abs(shortestPath)&&
            (holders[(current+i)%5]==HOLDER_RED || holders[(current+i)%5]==HOLDER_GREEN))
            shortestPath = i;
    }
    return shortestPath;
}

void Revolver::rotate(int count) {
    current += count;
}
int Revolver::totalRed()const{
    int current_totalRed = 0;
    for(int i=0; i<5; i++) {
        if (holders[i] == HOLDER_RED) {
            current_totalRed += 1;
        }
    }
    return current_totalRed;
}
int Revolver::totalGreen()const{
    int current_totalGreen = 0;
    for (int i=0;i<5;i++) {
        if (holders[i] == HOLDER_GREEN) {
            current_totalGreen += 1;
        }
    }
    return current_totalGreen;
}
bool Revolver::isEmpty()const{
    for (int i=0;i<5;i++) {
        if (holders[i] == HOLDER_GREEN || holders[i] == HOLDER_RED) {
            return false;
        }
    }
    return true;
}
void Revolver::addCup(bool red) {
    Holder cup;
    if (red == false) {
        cup = HOLDER_GREEN;
    }
    else {
        cup = HOLDER_RED;
    }
    holders[current%5] = cup;
}
void Revolver::setHolder(int n,Holder h){
    if(n==-1)holders[current%5]=h;
    else holders[n%5]=h;
}

std::ostream& operator<<(std::ostream& s,const Revolver& r){
    for(int i=0;i<5;++i){
        if(r.current%5 == i) s<<"~~";
        switch(r.holders[i]){
            case HOLDER_OPEN: s<<"OPEN";break;
            case HOLDER_WAITING: s<<"WAITING";break;
            case HOLDER_CLOSED: s<<"CLOSED";break;
            case HOLDER_RED: s<<"RED";break;
            case HOLDER_GREEN: s<<"GREEN";break;
        }
        if(r.current%5 == i) s<<"~~";
        if(i<4) s<<" | ";
    }
    return s;
}








