#ifndef CLAW_H
#define CLAW_H

#include <iostream>

enum Buoy{EMPTY,RED,GREEN};

class Claw{
    Buoy holding; //holding=EMPTY; if(holding==GREEN);
    bool height; // 1 is up, 0 is down
    bool rotation; // 1 is right way up, 0 is upside down
    //bool clamped;
public:
    Claw():holding(EMPTY),height(false),rotation(true){};
    bool isFull();
    Buoy currentContents();
    void addCup(bool isRed);
    void setUp(bool isUp){height = isUp;};
    void setRotation(bool rightWay){rotation=rightWay;};
    bool isUp();
    bool isClamped();
    void clamp(Buoy b);
    void release();
    friend std::ostream& operator<<(std::ostream& s,const Claw& c);
};

std::ostream& operator<<(std::ostream& s,const Claw& c);
#endif
