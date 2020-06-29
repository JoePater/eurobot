#include "claw.h"


bool Claw::isFull(){
    if(holding == EMPTY){
        return false;
    }
    else{
        return true;
    }
}

Buoy Claw::currentContents(){
    return holding;
}

void Claw::addCup(bool isRed){
    if(isRed == true){
        holding = RED;
    }
    else{
        holding = GREEN;
    }
}

bool Claw::isUp(){
    return height;
}

bool Claw::isClamped(){
    if(holding == EMPTY){
        return false;
    }
    else{
        return true;
    }
}
void Claw::clamp(Buoy b){
    holding = b;
}
void Claw::release(){
    holding = EMPTY;
}

std::ostream& operator<<(std::ostream& s,const Claw& c){
    s<<"Currently ";
    if(c.holding==EMPTY){
        s<<"empty";
    }else if(c.holding==RED){
        s<<"holding red";
    }else if(c.holding==GREEN){
        s<<"holding green";
    }
    s<<", claw is "<<(c.height?"up":"down")<<" and "<<(c.rotation?"right way up":"upside down");
    return s;
}






