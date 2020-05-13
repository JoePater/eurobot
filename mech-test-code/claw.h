#ifndef CLAW_H
#define CLAW_H

enum Buoy{EMPTY,RED,GREEN};

class Claw{
    Buoy holding; //holding=EMPTY; if(holding==GREEN);
    bool height; // 1 is up, 0 is down
    bool rotation; // 1 is right way up, 0 is upside down
    //bool clamped;
public:
    Claw()=default;
    bool isFull();
    Buoy currentContents();
    void addCup(bool isRed);
    void setUp(bool isUp){height = true;};
    void setRotation(bool rightWay){rotation=rightWay;};
    bool isUp();
    bool isClamped();
    void clamp(Buoy b);
    void release();
};
#endif
