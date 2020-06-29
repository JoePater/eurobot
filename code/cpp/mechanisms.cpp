#define DEBUG_MODE

#include "mechanisms.h"

#ifndef DEBUG_MODE
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#endif


Mechanisms::Mechanisms(){}

bool Mechanisms::sendBytes(int addr,int b1,int b2,int b3){
#ifndef DEBUG_MODE
    i2cFile = open("/dev/i2c-1",O_RDWR);
    if(i2cFile < 0) return 0;
    if(ioctl(i2cFile,I2C_SLAVE,addr)<0) return 0;
    unsigned char buf[5];
    int argc = 2;
    buf[0] = b1;
    buf[1] = 0;
    if(b2 != -1){
        buf[1] = b2;
        argc = 2;
    }if(b3 != -1){
        buf[2] = b3;
        argc = 3;
    }
    bool succeeded = (write(i2cFile,buf,argc)==argc);
    close(i2cFile);
    return succeeded;
#endif
#ifdef DEBUG_MODE
    std::cout<<"Sending bytes...\n";
    return true;
#endif
}
unsigned char Mechanisms::readByte(int addr,int reg){
#ifndef DEBUG_MODE
    if(reg != -1){
        if(!sendBytes(addr,reg))throw 1;
    }
    i2cFile = open("/dev/i2c-1",O_RDWR);
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
    std::cout<<"Reading bytes...\n";
    unsigned char c;
    std::cin>>c;
    return c;
#endif
}

bool Mechanisms::setDrive(float left,float right){
#ifdef DEBUG_MODE
    std::cout<<"Setting drive to "<<left<<", "<<right<<"...\n";
#endif
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
#ifdef DEBUG_MODE
    std::cout<<"Setting drive failed\n";
#endif
    return false;
}

std::string routineStr(MotorRoutine r){
    switch(r){
        case CLAW_DOWN: return "claw down";
        case CLAW_DOWN_EDGE: return "claw down edge";
        case CLAMP: return "clamp";
        case RELEASE: return "release";
        case CLAW_UP: return "claw up";
        case CLAW_UP_EDGE: return "claw up edge";
        case CLAW_HALF_WAY_UP: return "claw half way up";
        case CLAW_RIGHT_WAY_UP: return "claw right way up";
        case CLAW_UPSIDE_DOWN: return "claw upside down";
        case STOP_ALL: return "stop all";
    }
    return "---";
}

bool Mechanisms::setRoutine(MotorRoutine r,Buoy b){
#ifdef DEBUG_MODE
    std::cout<<"Setting routine to "<<routineStr(r)<<"...";
#endif
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
        bin = 0b11000110;
        claw.setUp(true);
        claw.setRotation(true);
        if(b!=EMPTY)revolver.addCup(b==RED);
        break;
    case CLAW_HALF_WAY_UP:
        bin = 0b11000111;
        claw.setUp(true);
        break;
    case CLAW_RIGHT_WAY_UP:
        bin = 0b11001000;
        claw.setRotation(true);
        break;
    case CLAW_UPSIDE_DOWN:
        bin = 0b11001001;
        claw.setRotation(false);
        break;
    case STOP_ALL:
        bin = 0b11001111;
        break;
    default:
        return 0;
    }
    if(sendBytes(MOTOR_BOARD_ADDR,bin,0)){
#ifdef DEBUG_MODE
        std::cout<<"Done\n";
#endif
        return true;
    }
#ifdef DEBUG_MODE
    std::cout<<"Failed\n";
#endif
    claw = backup;
    return false;
}

bool Mechanisms::setClawClamp(bool open,bool isRed){
#ifdef DEBUG_MODE
    std::cout<<"Setting claw to "<<(open?"open":(isRed?"red":"green"))<<"\n";
#endif
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
#ifdef DEBUG_MODE
    std::cout<<"Rotating revolver by "<<rot<<"\n";
#endif
    bool succeeded;
    if(rot <= revolver.clockwiseLimit() && rot >= revolver.anticlockwiseLimit()){
        if(rot >= 0)
            succeeded = sendBytes(MOTOR_BOARD_ADDR,0b11110000+rot,0);
        else
            succeeded = sendBytes(MOTOR_BOARD_ADDR,0b11100000-rot,0);
    }
    if(succeeded){
        revolver.rotate(rot);
    }
    return succeeded;
}

int Mechanisms::getClawProgress(){
#ifdef DEBUG_MODE
    std::cout<<"Getting claw progress: ";
    int i;
    std::cin>>i;
    return i;
#endif
#ifndef DEBUG_MODE
    return readByte(MOTOR_BOARD_ADDR,0b0100)&0b1111;
#endif
}

int Mechanisms::getClawRoutine(){
#ifdef DEBUG_MODE
    std::cout<<"Getting claw routine: ";
    int i;
    std::cin>>i;
    return i;
#endif
#ifndef DEBUG_MODE
    return (readByte(MOTOR_BOARD_ADDR,0b0100)>>4);
#endif
}

int Mechanisms::getRevProgress(){
#ifdef DEBUG_MODE
    std::cout<<"Getting revolver progress: ";
    int i;
    std::cin>>i;
    return i;
#endif
#ifndef DEBUG_MODE
    return readByte(MOTOR_BOARD_ADDR,0b0100)&0b1111;
#endif
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
#ifdef DEBUG_MODE
    std::cout<<"Setting cup holder "<<n<<" open\n";
#endif
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
#ifdef DEBUG_MODE
    std::cout<<"Setting cup holder "<<n<<" closed\n";
#endif
    bool succeeded=false;
    if(n==-1){
        succeeded = sendBytes(SERVO_MCU_ADDR,0b10000110+(revolver.currentPosition()<<4));
    }
    else if(n >= 0 && n <= 4){
        succeeded = sendBytes(SERVO_MCU_ADDR,0b10000110+(n<<4));
    }
    if(succeeded){
        revolver.setHolder(n,claw.currentContents()==RED?HOLDER_RED:HOLDER_GREEN);
    }
    return succeeded;
}
bool Mechanisms::setCupHolderWaiting(int n){
#ifdef DEBUG_MODE
    std::cout<<"Setting cup holder "<<n<<" waiting\n";
#endif
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
#ifdef DEBUG_MODE
    std::cout<<"Erecting flag\n";
#endif
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
    std::cout<<"Need cup color (E,R,G): ";
    std::string s;
    std::cin>>s;
    if(s[0]=='E')return EMPTY;
    if(s[0]=='R')return RED;
    if(s[0]=='G')return GREEN;
	return EMPTY;
#endif
}

void Mechanisms::displayNumber(int n){
#ifdef DEBUG_MODE
    std::cout<<"Displaying number "<<n<<"\n";
#endif
}

std::ostream& operator<<(std::ostream& s,const Mechanisms& m){
    return s<<"Claw: "<<m.claw<<"\nRevolver: "<<m.revolver;
}









