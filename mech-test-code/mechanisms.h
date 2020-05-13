#ifndef MECHANISMS_H
#define MECHANISMS_H

#include "claw.h"
#include "revolver.h"

const int MOTOR_BOARD_ADDR = 25;
const int SERVO_MCU_ADDR = 26;

enum MotorRoutine{
    CLAW_DOWN,CLAW_DOWN_EDGE,CLAMP,RELEASE,CLAW_UP,CLAW_UP_EDGE,STOP_ALL
};

class Mechanisms{
private:
    Revolver revolver;
    Claw claw;
    int routineCnt;
    bool sendBytes(int addr,int b1,int b2=-1,int b3=-1);
    unsigned char readByte(int addr,int reg=-1);
    int i2cFile;
public:
    Mechanisms(); //init sensors
    bool setDrive(float left,float right);
    bool setRoutine(MotorRoutine r,Buoy b=EMPTY);
    bool setClawClamp(bool open,bool isRed=true);
    //bool setClawOrientation(bool upsideDown);
    //bool setClawHeight(bool up,bool edge=false);
    bool setRevolverRotation(int rot);

    int getClawRoutine();
    int getClawProgress();
    int getRevProgress(); //0 is complete

    bool clawUp();
    bool clawDown();
    bool clawNormalRotation();
    bool clawUpsideDown();
    bool clawClamped();
    bool clawReleased();

    bool setCupHolderOpen(int n); //-1 is current
    bool setCupHolderWaiting(int n); //-1 is current
    bool setCupHolderClosed(int n); //-1 is current
	
	bool erectFlag();
    bool cupHolderPressed(int n);
    bool cupHolderClosed(int n);
    bool cupHolderWaiting(int n);

    const Revolver& getRevolver()const{return revolver;};
    const Claw& getClaw()const{return claw;};
    //const Drive& getDrive()const{return drive;};

    Buoy getCupColor();
};









#endif
