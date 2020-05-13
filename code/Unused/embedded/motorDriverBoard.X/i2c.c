
#include <pic16f18875.h>
#include "i2c.h"
#include "system.h"
#include "settings.h"

enum i2c_stat{
    I2C_OPTIONS,
    I2C_VALUE,
    I2C_VALUE2
};

extern unsigned char i2c_status;
extern unsigned char i2c_settings;
extern unsigned char i2c_val;
extern struct properties props;

void i2cISR(){
    unsigned char d = 0;
    SSP1IF = 0;
    d = SSP1BUF;
    if(SSP1CON3bits.ACKTIM){ //before acknowledge
        if(SSP1STATbits.D_nA){ //if data
            if(SSP1STATbits.R_nW == 0) //if write
                handleWrite(d);
        }
    }else{
        if(!SSP1STATbits.D_nA){ //if address
            i2c_status = I2C_OPTIONS;
            if(SSP1STATbits.R_nW == 0) //if write
                i2c_settings = 0;
            else
                handleRead();
        }
    }
    SSP1CON2bits.ACKDT = 0;
    SSP1CON1bits.CKP = 1; //release clock
}

void handleWrite(unsigned char d){
    switch(i2c_status){
        case I2C_OPTIONS:
            i2c_settings = d;
            if(!i2c_settings)LATEbits.LATE2 = 1;
            i2c_status = I2C_VALUE;
            break;
        case I2C_VALUE:
            i2c_val = d;
            setStatus(i2c_settings,i2c_val,-1);
            i2c_status = I2C_VALUE2;
            break;

        case I2C_VALUE2:
            setStatus(i2c_settings,i2c_val,(int)d);
            i2c_status = I2C_OPTIONS;
            break;
        default:
            i2c_status = I2C_VALUE;
            i2c_settings = d;
            break;
    }
}

void handleRead(){
    switch(i2c_settings&0b1111){
        case 0b0000:
            SSP1BUF = props.claw1.height;
            break;
        case 0b0001:
            SSP1BUF = props.claw1.orientation;
            break;
        case 0b0010:
            SSP1BUF = props.claw1.clamped;
            break;
        case 0b0011:
            SSP1BUF = props.revolver.stat;
            break;
        case 0b0100:
            SSP1BUF = props.claw1.routine;
            break;
        case 0b0101:
            SSP1BUF = props.revolver.stat;
            break;
        default:
            SSP1BUF = 0;
            break;
    }
}


/* bits 7:6 = write?,routine?
 * bits 7:6 = 10 {
 * bits 5:3 = motor selection
 *  000 - drive motors
 *  001 - motor 0 = revolver motor
 *  010 - motor 1 = claw movement
 *  011 - motor 2 = claw rotate
 *  100 - motor 3 = claw clamp
 * bits 2:0 = direction control
 *  000 - forwards
 *  001 - backwards
 *  010 - clockwise on the spot
 *  011 - anticlockwise on the spot
 * }
 * bits 7:6 = 11 {
 * bit 5 = revolver rotate?
 * bit 4 = rotate clockwise, 3:0=no. revs
 * bits 3:0 = subroutine selection
 *  0000 - none
 *  0001 - claw down (normal)
 *  0010 - claw down (for edge cups)
 *  0011 - claw grip
 *  0100 - claw release
 *  0101 - claw up
 *  0110 - claw up with edge cup
 *  1111 - stop everything
 * 
 *  
 * }
 * bits 7:6 = 0x {
 * bits 3:0 = read selection
 *  0000 - get claw height
 *  0001 - get claw rotation
 *  0010 - get claw clamp
 *  0011 - get revolver info
 *  0100 - get claw routine
 *  0101 - get revolver routine
 * }
*/