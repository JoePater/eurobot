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
