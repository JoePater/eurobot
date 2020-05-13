
#include "settings.h"
#include "system.h"

extern struct properties props;
extern unsigned char motorDirArr[4];
extern unsigned char motorPWMArr[4];
extern unsigned char driveDir;
extern unsigned char drivePWM1;
extern unsigned char drivePWM2;

void setStatus(unsigned char s,unsigned char v,int v2){
    unsigned char dir = s&0b111;
    unsigned char motor = (s&0b111000)>>3;
    if(v2 >= 0){
        
        setDrive2(dir,v,v2);
        return;
    }
    if(s & 0b01000000){ //routine
        
        if(!(s & 0b00100000)) //not rev
            setRoutine(s);
        else
            revRoutine(s);
    }
    else{
        if(s & 0b111000){ //not drive motors
            //enable overrides
            LATEbits.LATE2 = 0;
            if(motor==1)props.revolver.stat |= 0x10;
            else if(motor==2)props.claw1.height |= 0x8;
            else if(motor==3)props.claw1.orientation |= 0x8;
            else if(motor==4)props.claw1.clamped |= 0x8;
            setMotors(motor,dir,v);
        }
        else{ //drive motors
            setDrive(dir,v);
        }
    }
}

void setPWM(unsigned char module,unsigned char pwm){
    pwm = pwm<=250 ? pwm : 250;
    switch(module){
        case 1:
            CCPR1H = pwm;
            break;
        case 2:
            CCPR2H = pwm;
            break;
        case 3:
            CCPR3H = pwm;
            break;
        case 4:
            CCPR4H = pwm;
            break;
        case 5:
            CCPR5H = pwm;
            break;
        case 6:
            PWM6DCH = pwm;
            break;
        case 7:
            PWM7DCH = pwm;
            break;
    }
}

void setRoutine(unsigned char s){
    if(s&0b1111)
        props.claw1.time1 = props.time;
    //turn off override
    //LATEbits.LATE2 = 1;
    props.claw1.clamped &= 0b11110111;
    props.claw1.orientation &= 0b11110111;
    props.claw1.height &= 0b11110111;
    switch(s&0b1111){
        case 0b0000:
            props.claw1.routine = 0;
            break;
        case 0b0001:
            setMotors(2,1,64); //lift,backwards
            props.claw1.routine = 0x11;
            break;
        case 0b0010:
            setMotors(2,1,64); //lift,backwards
            props.claw1.routine = 0x24;
            break;
        case 0b0011:
            setMotors(4,1,64); //clamp,backwards
            props.claw1.routine = 0x31;
            break;
        case 0b0100:
            setMotors(4,0,64); //clamp,forwards
            props.claw1.routine = 0x41;
            break;
        case 0b0101:
            setMotors(2,0,64); //claw lift,forwards
            setMotors(3,0,64); //rotate,forwards
            props.claw1.routine = 0x52;
            break;
        case 0b0110:
            props.claw1.routine = 0x64;
            break;
        case 0b1111:
            for(char i=0;i<5;++i)
                setMotors(i,0,0);
            break;
    }
}

void revRoutine(unsigned char s){
    //0b11110001
    props.revolver.stat = ((s<<2)&0b11000000)+(s&0b1111);
    if(s&0x40)//clockwise
        setMotors(1,1,255);
    else
        setMotors(1,0,255);
}

void setMotors(unsigned char motor,unsigned char dir,unsigned char v){
    dir = dir!=0;
    if((motorDirArr[motor] != dir) || (motorPWMArr[motor] != v)){
        motorDirArr[motor] = dir;
        motorPWMArr[motor] = v;
        switch(motor){
            case 1:
                TRISC |= 0b01010101;
                RC6PPS = 9*(!dir);
                RC2PPS = 0;
                RC4PPS = 9*dir;
                RC0PPS = 0;
                LATCbits.LATC6 = !dir;
                LATCbits.LATC2 = dir;
                LATCbits.LATC4 = dir;
                LATCbits.LATC0 = !dir;
                setPWM(1,v);
                TRISC &= 0b10101010;
                break;
            case 0b10:
                TRISC |= 0b10101010;
                RC7PPS = 10*(!dir); //top left
                RC3PPS = 0; //bottom left
                RC5PPS = 10*dir; //top right
                RC1PPS = 0; //bottom right
                LATCbits.LATC7 = !dir;
                LATCbits.LATC3 = dir;
                LATCbits.LATC5 = dir;
                LATCbits.LATC1 = !dir;
                setPWM(2,v);
                TRISC &= 0b01010101;
                break;
            case 0b11:
                TRISD |= 0b01010101;
                RD4PPS = 11*(!dir);
                RD0PPS = 0;
                RD6PPS = 11*dir;
                RD2PPS = 0;
                LATDbits.LATD4 = !dir;
                LATDbits.LATD0 = dir;
                LATDbits.LATD6 = dir;
                LATDbits.LATD2 = !dir;
                setPWM(3,v);
                TRISD &= 0b10101010;
                break;
            case 0b100:
                TRISD |= 0b01010101;
                RD5PPS = 12*(!dir);
                RD1PPS = 0;
                RD7PPS = 12*dir;
                RD3PPS = 0;
                LATDbits.LATD5 = !dir;
                LATDbits.LATD1 = dir;
                LATDbits.LATD7 = dir;
                LATDbits.LATD3 = !dir;
                setPWM(4,v);
                TRISD &= 0b10101010;
                break;
        }
    }
}

void setDrive(unsigned char dir,unsigned char v){
    if((driveDir != dir) || (drivePWM1 != v) || (drivePWM2 != v)){
        driveDir = dir;
        drivePWM1 = v;
        drivePWM2 = v;
        switch(dir){
            case 0b000: //forwards
                TRISA |= 0b00111111;
                RA0PPS = 13; //CCP5
                RA1PPS = 0x23; //CWG3D
                RA2PPS = 14; //PWM6
                RA3PPS = 0; //off
                RA4PPS = 0x22; //CWG3C
                RA5PPS = 0; //off
                LATA &= 0b11000000;
                CWG3ISM = 0b0111; //PWM7
                setPWM(5,v);
                setPWM(6,v);
                setPWM(7,245);
                TRISA &= 0b11000000;
                break;
            case 0b001: //backwards
                TRISA |= 0b00111111;
                CWG3ISM = 0b0101; //CCP5
                CWG1ISM = 0b0110; //PWM6
                RA0PPS = 0x23; //CWG3D (inverted)
                RA1PPS = 0; //on
                RA2PPS = 0x02; //CLC2 (inverted)
                RA3PPS = 0x22; //CWG3C
                RA4PPS = 0; //off
                RA5PPS = 0x01; //CLC1
                LATA &= 0b11000000;
                LATA |= 0b00000010;
                setPWM(5,v);
                setPWM(6,v);
                TRISA &= 0b11000000;
                break;
            case 0b010: //clockwise
                TRISA |= 0b00111111;
                CWG3ISM = 0b0101; //CCP5
                RA0PPS = 0; //on
                RA1PPS = 0x23;//0; //off
                RA2PPS = 0x23; //CWG3D (inverted)
                RA3PPS = 0; //off
                RA4PPS = 0; //off
                RA5PPS = 0x22; //CWG3C
                LATA &= 0b11000000;
                LATA |= 0b00000001;
                setPWM(5,v);
                TRISA &= 0b11000000;
                break;
            case 0b011:
                TRISA |= 0b00111111;
                CWG3ISM = 0b0101; //CCP5
                RA0PPS = 0x23; //CWG3D (inverted)
                RA1PPS = 0x23;// CWG3D (inverted)
                RA2PPS = 0; //on
                RA3PPS = 0x22; //CWG3C
                RA4PPS = 0; //off
                RA5PPS = 0; //off
                LATA &= 0b11000000;
                LATA |= 0b00000100;
                setPWM(5,v);
                TRISA &= 0b11000000;
                break;
        }
    }
}
void setDrive2(unsigned char dir,unsigned char v,unsigned char v2){
    if((driveDir != dir) || (drivePWM1 != v) || (drivePWM2 != v2)){
        driveDir = dir;
        drivePWM1 = v;
        drivePWM2 = v2;
        switch(dir){
            case 0b000: //forwards
                TRISA |= 0b00111111;
                RA0PPS = 13; //CCP5
                RA1PPS = 0x23; //CWG3D
                RA2PPS = 14; //PWM6
                RA3PPS = 0; //off
                RA4PPS = 0x22; //CWG3C
                RA5PPS = 0; //off
                LATA &= 0b11000000;
                CWG3ISM = 0b0111; //PWM7
                setPWM(5,v);
                setPWM(6,v2);
                setPWM(7,245);
                TRISA &= 0b11000000;
                break;
            case 0b001: //backwards
                TRISA |= 0b00111111;
                CWG3ISM = 0b0101; //CCP5
                CWG1ISM = 0b0110; //PWM6
                RA0PPS = 0x23; //CWG3D (inverted)
                RA1PPS = 0; //on
                RA2PPS = 0x02; //CLC2 (inverted)
                RA3PPS = 0x22; //CWG3C
                RA4PPS = 0; //off
                RA5PPS = 0x01; //CLC1
                LATA &= 0b11000000;
                LATA |= 0b00000010;
                setPWM(5,v);
                setPWM(6,v2);
                TRISA &= 0b11000000;
                /*TRISA |= 0b00111111;
                //RA0+2 = inverted CWG of highest PWM
                //CCP5 = higher,PWM6 = lower
                CWG3ISM = 0b0101; //CCP5
                RA0PPS = 0x23; //CWGD (inverted)
                RA1PPS = 0; //on
                RA2PPS = 0x23; //CWGD (inverted)
                RA4PPS = 0; //off
                if(v>v2){
                    RA3PPS = 0x21; //CWGB
                    RA5PPS = 0x01; //CLC1
                    setPWM(5,v);
                    setPWM(6,v2);
                }else{
                    RA3PPS = 0x01; //CLC1
                    RA5PPS = 0x21; //CWGB
                    setPWM(5,v2);
                    setPWM(6,v);
                }
                LATA &= 0b11000000;
                LATA |= 0b00000010;
                TRISA &= 0b11000000;*/
                break;
        }
    }
}
