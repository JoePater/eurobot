#include "mechanisms.h"
#include <iostream>

#define DEBUG_MODE

#ifndef DEBUG_MODE
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#endif


Mechanisms::Mechanisms(){}

bool Mechanisms::sendBytes(int addr,int b1,int b2,int b3){
#ifndef DEBUG_MODE
    i2cFile = open("/dev/i2c-1",0_RDWR);
    if(i2cFile < 0) return 0;
    if(ioctl(i2cFile,I2C_SLAVE,addr)<0) return 0;
    unsigned char buf[5];
    int argc = 1;
    buf[0] = b1;
    if(b2 != -1){
        buf[1] = b2;
        argc = 2;
    }if(b3 != -1){
        buf[2] = b3;
        argc = 3;
    }
    bool succeeded = (write(i2cFile,buf,argc)==argc);
    close(i2cFile);
#endif
#ifdef DEBUG_MODE
	std::cout<<"Sending bytes... \n";
	return true;
#endif
}
unsigned char Mechanisms::readByte(int addr,int reg){
#ifndef DEBUG_MODE
    if(reg != -1){
        if(!sendBytes(addr,reg))throw 1;
    }
    i2cFile = open("/dev/i2c-1",0_RDWR);
    if(i2cFile < 0) throw 1;
    if(ioctl(i2cFile,I2C_SLAVE,addr)<0){
        close(i2cFile);
        throw 1;
    }
    unsigned char res;
    bool succeeded = read(i2cFile,&res,1)==1;
    close(i2cFile);
    if(succeeded)return res;
    throw 1;
#endif
#ifdef DEBUG_MODE
	std::cout<<"Reading bytes... \n";
	return 0;
#endif
}

bool Mechanisms::setDrive(float left,float right){
    if(left >= 0 && right >= 0){
        if(sendBytes(MOTOR_BOARD_ADDR,128,left,right)){
            return true;
        }
    }else if(left <= 0 && right <= 0){
        if(sendBytes(MOTOR_BOARD_ADDR,129,left,right)){
            return true;
        }
    }else if(left > 0 && right==-left){
        if(sendBytes(MOTOR_BOARD_ADDR,130,left)){
            return true;
        }
    }else if(right > 0 && left==-right){
        if(sendBytes(MOTOR_BOARD_ADDR,131,right)){
            return true;
        }
    }
    return false;
}

bool Mechanisms::setRoutine(MotorRoutine r,Buoy b){
    int bin = 0;
    Claw backup = claw;
    switch(r){
    case CLAW_DOWN:
        bin = 0b11000001;
        claw.setUp(false);
        claw.setRotation(true);
        if(claw.isClamped())revolver.setHolder(-1,HOLDER_OPEN);
        break;
    case CLAW_DOWN_EDGE:
        bin = 0b11000010;
        claw.setUp(false);
        claw.setRotation(false);
        if(claw.isClamped())revolver.setHolder(-1,HOLDER_OPEN);
        break;
    case CLAMP:
        bin = 0b11000011;
        claw.clamp(b);
        break;
    case RELEASE:
        bin = 0b11000100;
        claw.release();
        break;
    case CLAW_UP:
        bin = 0b11000101;
        claw.setUp(true);
        claw.setRotation(true);
        if(b!=EMPTY)revolver.addCup(b==RED);
        break;
    case CLAW_UP_EDGE:
        bin = 0b11000101;
        claw.setUp(true);
        claw.setRotation(true);
        if(b!=EMPTY)revolver.addCup(b==RED);
        break;
    case STOP_ALL:
        bin = 0b11001111;
        break;
    default:
        return 0;
    }
    if(sendBytes(MOTOR_BOARD_ADDR,bin,0)){
        return true;
    }
    claw = backup;
    return false;
}

bool Mechanisms::setClawClamp(bool open,bool isRed){
    bool succeeded;
    if(open)
        succeeded = sendBytes(MOTOR_BOARD_ADDR,0b11000100,0);
    else
        succeeded = sendBytes(MOTOR_BOARD_ADDR,0b11000011,0);
    if(succeeded){
        if(open) claw.release();
        else claw.clamp(isRed ? RED : GREEN);
    }
    return succeeded;
}
/*
bool Mechanisms::setClawHeight(bool up,bool edge){
    bool succeeded;
    if(up){
        if(edge)
            succeeded = sendBytes(MOTOR_BOARD_ADDR,0b11000110,0);
        else
            succeeded = sendBytes(MOTOR_BOARD_ADDR,0b11000101,0);
    }else{
        if(edge)
            succeeded = sendBytes(MOTOR_BOARD_ADDR,0b11000010,0);
        else
            succeeded = sendBytes(MOTOR_BOARD_ADDR,0b11000001,0);
    }
    if(succeeded){
        claw.setUp(up);
        claw.setRotation(!edge);
    }
    return succeeded;
}
*/
bool Mechanisms::setRevolverRotation(int rot){
    bool succeeded;
    if(rot <= revolver.clockwiseLimit() && rot >= revolver.anticlockwiseLimit()){
        if(rot >= 0)
            succeeded = sendBytes(MOTOR_BOARD_ADDR,0b11110000+rot,0);
        else
            succeeded = sendBytes(MOTOR_BOARD_ADDR,0b11100000-rot,0); //negative
    }
    if(succeeded){
        revolver.rotate(rot);
    }
    return succeeded;
}

int Mechanisms::getClawProgress(){
    return readByte(MOTOR_BOARD_ADDR,0b0100)&0b1111;
}

int Mechanisms::getClawRoutine(){
    return (readByte(MOTOR_BOARD_ADDR,0b0100)>>4);
}

int Mechanisms::getRevProgress(){
    return readByte(MOTOR_BOARD_ADDR,0b0101)&0b1111;
}

bool Mechanisms::clawUp(){
    unsigned char x = readByte(MOTOR_BOARD_ADDR,0b0000);
    if(x&0b00100000)return true;
    return false;
}
bool Mechanisms::clawDown(){
    unsigned char x = readByte(MOTOR_BOARD_ADDR,0b0000);
    if(x&0b00010000)return true;
    return false;
}
bool Mechanisms::clawNormalRotation(){
    unsigned char x = readByte(MOTOR_BOARD_ADDR,0b0001);
    if(x&0b00100000)return true;
    return false;
}
bool Mechanisms::clawUpsideDown(){
    unsigned char x = readByte(MOTOR_BOARD_ADDR,0b0001);
    if(x&0b00010000)return true;
    return false;
}
bool Mechanisms::clawClamped(){
    unsigned char x = readByte(MOTOR_BOARD_ADDR,0b0010);
    if(x&0b00100000)return true;
    return false;
}
bool Mechanisms::clawReleased(){
    unsigned char x = readByte(MOTOR_BOARD_ADDR,0b0010);
    if(x&0b00010000)return true;
    return false;
}

bool Mechanisms::setCupHolderOpen(int n){
    bool succeeded=false;
    if(n==-1){
        succeeded = sendBytes(SERVO_MCU_ADDR,0b10001010+(revolver.currentPosition()<<4));
    }
    else if(n >= 0 && n <= 4){
        succeeded = sendBytes(SERVO_MCU_ADDR,0b10001010+(n<<4));
    }
    if(succeeded){
        revolver.setHolder(n,HOLDER_OPEN);
    }
    return succeeded;
}
bool Mechanisms::setCupHolderClosed(int n){
    bool succeeded=false;
    if(n==-1){
        succeeded = sendBytes(SERVO_MCU_ADDR,0b10000110+(revolver.currentPosition()<<4));
    }
    else if(n >= 0 && n <= 4){
        succeeded = sendBytes(SERVO_MCU_ADDR,0b10000110+(n<<4));
    }
    if(succeeded){
        revolver.setHolder(n,HOLDER_CLOSED);
    }
    return succeeded;
}
bool Mechanisms::setCupHolderWaiting(int n){
    bool succeeded=false;
    if(n==-1){
        succeeded = sendBytes(SERVO_MCU_ADDR,0b10001110+(revolver.currentPosition()<<4));
    }
    else if(n >= 0 && n <= 4){
        succeeded = sendBytes(SERVO_MCU_ADDR,0b10001110+(n<<4));
    }
    if(succeeded){
        revolver.setHolder(n,HOLDER_WAITING);
    }
    return succeeded;
}
bool Mechanisms::erectFlag(){
    bool succeeded=sendBytes(SERVO_MCU_ADDR,0b10);
    return succeeded;
}
bool Mechanisms::cupHolderPressed(int n){
    unsigned char x=0;
    if(n==-1){
        x=readByte(SERVO_MCU_ADDR,0b10000000+(revolver.currentPosition()<<4));
    }else if(n>=0 && n<=4){
        x=readByte(SERVO_MCU_ADDR,0b10000000+(n<<4));
    }
    return (x&0b01000000)>>6;
}
bool Mechanisms::cupHolderClosed(int n){
    unsigned char x=0;
    if(n==-1){
        x=readByte(SERVO_MCU_ADDR,0b10000000+(revolver.currentPosition()<<4));
    }else if(n>=0 && n<=4){
        x=readByte(SERVO_MCU_ADDR,0b10000000+(n<<4));
    }
    return !((x&0b10000000)>>7);
}
bool Mechanisms::cupHolderWaiting(int n){
    unsigned char x=0;
    if(n==-1){
        x=readByte(SERVO_MCU_ADDR,0b10000000+(revolver.currentPosition()<<4));
    }else if(n>=0 && n<=4){
        x=readByte(SERVO_MCU_ADDR,0b10000000+(n<<4));
    }
    return (x&0b00100000)>>5;
}

Buoy Mechanisms::getCupColor(){
#ifndef DEBUG_MODE
#endif
#ifdef DEBUG_MODE
	return EMPTY;
#endif
}



















//
