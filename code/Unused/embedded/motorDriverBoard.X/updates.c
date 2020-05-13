
#include "updates.h"
#include "system.h"
#include "settings.h"

#define AVG_CLAMP_TIME 1000
#define AVG_ROTATE_TIME 1500
#define AVG_LIFT_TIME 1300
#define F_BIT 0b01000000

extern struct properties props;


char getProgress(unsigned int t,unsigned int total,char curr){
    for(;(curr*(total>>4))<t;curr++);
    return curr;
}

void updateSys(struct properties *p){
    
    //update buttons
    updateTime(p);
    testButtons(p);
    //if(p->claw1.height&0b10000)LATEbits.LATE2 = 1;
    //else LATEbits.LATE2 = 0;
    //update absolute positions + movement
    updateTime(p);
    updateMovement(p);
    
    //update physical outputs
    updateTime(p);
    updateMotors(p);
}

void updateMotors(struct properties *p){
    unsigned char *clamp = &(p->claw1.clamped);
    unsigned char *rotate = &(p->claw1.orientation);
    unsigned char *lift = &(p->claw1.height);
    if(!(*clamp&0x8)){
        if(*clamp & 0b10000000)
            setMotors(4,(*clamp&0b01000000) != 0,64);
        else setMotors(4,0,0);
    }
    if(!(*rotate&0x8)){
        if(*rotate & 0b10000000)
            setMotors(3,(*rotate&0b01000000) != 0,64);
        else setMotors(3,0,0);
    }
    if(!(*lift&0x8)){
        if(*lift & 0b10000000)
            setMotors(2,(*lift&0b01000000) != 0,64);
        else setMotors(2,0,0);
    }
    /*if(!(p->revolver.stat&0x10)){
        if(p->revolver.stat & 0b10000000)
            setMotors(1,(p->revolver.stat&0b01000000) != 0,255);
        else setMotors(1,0,0);
    }*/
}

void updateMovement(struct properties *p){
    unsigned char *clamp = &(p->claw1.clamped);
    unsigned char *rotate = &(p->claw1.orientation);
    unsigned char *lift = &(p->claw1.height);
    
    /*
    //update absolute positions
    if(*clamp & 0b00100000) //claw clamped
        *clamp |= 0b01000000;
    if(*rotate & 0b00100000) //normal orientation
        *rotate |= 0b01000000;
    if(*lift & 0b00100000) //up
        *lift |= 0b01000000;
    
    if(*clamp & 0b00010000) //claw open
        *clamp &= 0b10111111;
    if(*rotate & 0b00010000) //upside down
        *rotate &= 0b10111111;
    if(*lift & 0b00010000) //down
        *lift &= 0b10111111;
    */
    
    //check for routines, s=status
    unsigned char s = p->claw1.routine & 0b1111;
    if(s){
        //switch routine type
        switch((p->claw1.routine & 0b11110000)>>4){
            case 1: //move down
                //LATEbits.LATE2 = 1;
                if(*lift&0b10000){ //stop switch
                    //LATEbits.LATE2 = 0;
                    --p->claw1.routine;
                    setProps(0,0,0);
                }
                else
                    setProps(0b10,0b00,0b00);
                break;
            case 2: //claw down for edge
                switch(s){
                    case 4:
                        //going down for 500ms
                        if(p->time-p->claw1.time1 > 500)
                            --p->claw1.routine;
                        else
                            setProps(0b10,0b00,0b00);
                        break;
                    case 3:
                        //down and rotating for 500ms or until upside down
                        if(p->time-p->claw1.time1 > 1000)
                            --p->claw1.routine;
                        else if(*rotate & 0x10)//upside down
                            p->claw1.routine -= 2;
                        else
                            setProps(0b10,0b10,0b00);
                        break;
                    case 2:
                        //rotating
                        if(*rotate & 0x10)
                            --p->claw1.routine;
                        else
                            setProps(0b00,0b10,0b00);
                        break;
                    case 1:
                        //down
                        if(*lift & 0x10){
                            --p->claw1.routine;
                            setProps(0b00,0b00,0b00);
                        }else
                            setProps(0b10,0b00,0b00);
                        break;
                }
                break;
            case 3: //grip claw
                if(*clamp&0b100000){ //stop switch
                    --p->claw1.routine;
                    setProps(0b00,0b00,0b00);
                }else
                    setProps(0b00,0b00,0b11);
                break;
            case 4: //release claw
                if(*clamp&0b10000){ //stop switch
                    --p->claw1.routine;
                    setProps(0b00,0b00,0b00);
                }else
                    setProps(0b00,0b00,0b10);
                break;
            case 5: //claw up
                if(s==2){
                    if(*rotate & 0x20 || *lift & 0x20) //normal orientation/up
                        --p->claw1.routine;
                    else
                        setProps(0b11,0b11,0);
                }else if(s==1){
                    if(*lift &0x20){
                        --p->claw1.routine;
                        setProps(0,0,0);
                    }else
                        setProps(0b11,0,0);
                }
                break;
            case 6: //claw up for edge
                switch(s){
                    case 4: //up for 300ms
                        if(p->time-p->claw1.time1 > 300)
                            --p->claw1.routine;
                        else
                            setProps(0b11,0,0);
                        break;
                    case 3: //up and rotate for 300ms
                        if(p->time-p->claw1.time1 > 600)
                            --p->claw1.routine;
                        else
                            setProps(0b11,0b11,0);
                        break;
                    case 2: //rotate until normal orientation
                        if(*rotate & 0x20)
                            --p->claw1.routine;
                        else
                            setProps(0,0b11,0);
                        break;
                    case 1: //up until top
                        if(*lift & 0x20){
                            --p->claw1.routine;
                            setProps(0,0,0);
                        }else
                            setProps(0b11,0,0);
                        break;
                }
                break;
        }
    }
    if(p->revolver.stat & 0x80){ //revolver rotate?
        //p->revolver.stat |= 0x20;
        if(p->revolver.stat & 0xF){ //if progress not 0
            if(p->revolver.stat&0x40)//clockwise
                setMotors(1,1,255);
            else
                setMotors(1,0,255);
            if((p->time-p->revolver.time1 > 250)&&(p->revolver.stat&0x20)){
                //button pressed and >250ms since last press
                --p->revolver.stat;
                p->revolver.time1 = p->time;
            }
        }
        else{
            p->revolver.stat &= 0x7F; //turn off motor
            setMotors(1,0,0);
        }
    }
    
    //update movement (turn off motor if reached button)
    if(((*clamp&0x40)&&(*clamp&0b00100000)) | (!(*clamp&0x40)&&(*clamp&0b00010000)))
        *clamp &= 0b01111111;
    if(((*rotate&0x40)&&(*rotate&0b00100000)) | (!(*rotate&0x40)&&(*rotate&0b00010000)))
        *rotate &= 0b01111111;
    if(((*lift&0x40)&&(*lift&0b00100000)) | (!(*lift&0x40)&&(*lift&0b00010000)))
        *lift &= 0b01111111;
    
}

void setProps(unsigned char l,unsigned char r,unsigned char c){
    props.claw1.height |= l<<6; //l=01, 0b01000000
    props.claw1.height &= ~((~l)<<6); //01111111
    
    props.claw1.orientation |= r<<6;
    props.claw1.orientation &= ~((~r)<<6);
    
    props.claw1.clamped |= c<<6;
    props.claw1.clamped &= ~((~c)<<6);
}

void testButtons(struct properties *p){
    unsigned char *clamp = &(p->claw1.clamped);
    unsigned char *rotate = &(p->claw1.orientation);
    unsigned char *lift = &(p->claw1.height);
    
    //Test for button changes
    if(PORTBbits.RB0 == 0) //clamped
        *clamp |= 0b00100000;
    else
        *clamp &= 0b11011111;
    
    if(PORTBbits.RB1 == 0) //released
        *clamp |= 0b00010000;
    else
        *clamp &= 0b11101111;
    
    if(PORTBbits.RB2 == 0) //right way up
        *rotate |= 0b00100000;
    else
        *rotate &= 0b11011111;
    
    if(PORTBbits.RB3 == 0) //upside down
        *rotate |= 0b00010000;
    else
        *rotate &= 0b11101111;
    
    if(PORTAbits.RA6 == 0) //claw up
        *lift |= 0b00100000;
    else
        *lift &= 0b11011111;
    
    if(PORTAbits.RA7 == 0){
        //LATEbits.LATE2 = 1;
        *lift |= 0b00010000;
    }else{
        *lift &= 0b11101111;
        //LATEbits.LATE2 = 0;
    }
    
    if(PORTEbits.RE1) //revolver active
        p->revolver.stat |= 0b00100000;
    else
        p->revolver.stat &= 0b11011111;
}

void updateTime(struct properties *p){
    unsigned int tPrev = p->time % 0x10000;
    unsigned int tNow = TMR0L;
    tNow += TMR0H<<8;
    if(tNow >= tPrev){ //timer hasn't overflowed
        p->time = (p->time & 0xFFFF0000) + tNow;
    }else{ //timer overflowed
        p->time = (p->time & 0xFFFF0000) + tNow + 0x10000;
    }
}
