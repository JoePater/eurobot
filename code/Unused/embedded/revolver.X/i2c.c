
#include "i2c.h"

extern unsigned char i2c_settings;
extern char flagUp;

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
        if(!SSP1CON3bits.ACKTIM){ //if address
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
    i2c_settings = d;
    checkData(d);
}

void checkData(unsigned char s){
    unsigned char stat = (s&0b1100)>>2;
    unsigned char holderSel = (s&0b01110000)>>4;
    
    holder *h = &harr[holderSel];
    switch(stat){
        case 1: //close
            *h &= 0b01011111;
            break;
        case 2: //open
            *h |= 0b10000000;
            *h &= 0b10100000;
            break;
        case 3: //open and waiting
            *h |= 0b11000000;
            break;
    }
    if(s&0b10){ //flag
        flagUp = 1;
    }
}

void handleRead(){
    unsigned char holderSel = (i2c_settings & 0b01110000)>>4;
    SSP1BUF = harr[holderSel];
}


/* bit 7 = servo?
 * bits 6:4 = holder select
 * bit 3:2 = set status (no change,closed,open,open+waiting for cup)
 * bit 1 = flag (0 = no change, 1 = up)
*/