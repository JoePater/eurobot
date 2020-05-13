


// PIC16F1826 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = ON        // Internal/External Switchover (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = ON       // PLL Enable (4x PLL enabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

#include "i2c.h"


#define FLAG_UP 11
#define CLOSED_PULSE 21
#define FLAG_DOWN 28
#define OPEN_PULSE 36


#define setbit(reg,bit,val) reg|=(val<<bit); reg&=~(!val<<bit);

holder harr[5];
//unsigned char i2c_status;
unsigned char i2c_settings;
//unsigned char i2c_val;
char flagUp;

unsigned char debug;


void __interrupt() ISR(void){
    if(SSP1IF == 1){ //if i2c interrupt
        i2cISR();
    }
}

void main(void){
    OSCCON = 0b01111010; //16MHz clock
    OPTION_REG = 0b00000111; //256 pre-scale,61Hz overflow
    INTCON = 0b11000000;
    PIE1 = 0b00001000;
    /* B1 = SDA, B4 = SCL
     * B0,B2,B3,B5,B6 = holder buttons
     * A0:4 = holder servos
     * A6 = flag servo
     */
    TRISA = 0b00100000;
    TRISB = 0b01111111;
    ANSELA = 0;
    ANSELB = 0;
    LATA = 0;
    WPUB = 0b01111111;
    
            
    SSP1STAT = 0b01000000;
    SSP1CON1 = 0b00100110;
    SSP1CON2 = 0b00000001;
    SSP1CON3 = 0b00101011;
    SSP1ADD = 0b00110100; //address = 0b0011010 = 26
    
    flagUp = 0;
    unsigned char flag = 0b11111111;
    
    for(char i=0;i<5;i++){
        harr[i] = 0b10000000;
    }
    
    unsigned char lataClosed;
    
    
    while(1){
        
        if(TMR0 > 240){
            while(TMR0 > 240);
            LATA |= 0b01101111;
            while(TMR0 < FLAG_UP);
            LATA &= flag;
            while(TMR0 < CLOSED_PULSE);
            LATA &= lataClosed;
            while(TMR0 < FLAG_DOWN);
            LATA &= 0b10111111;
            while(TMR0 < OPEN_PULSE);
            LATA &= 0b10100000;
        }
        setbit(harr[0],5,!PORTBbits.RB0);
        setbit(harr[1],5,!PORTBbits.RB2);
        setbit(harr[2],5,!PORTBbits.RB3);
        setbit(harr[3],5,!PORTBbits.RB5);
        setbit(harr[4],5,!PORTBbits.RB6);
        
        lataClosed = 0b11111111;
        
        for(int i=0;i<5;i++){
            holder *h = &harr[i];
            if((*h & 0b01100000)==0b01100000){ //waiting for cup and button pressed
                *h &= 0b00111111; //clear open and waiting
            }if(!(*h & 0b10000000)) //if closed
                lataClosed &= ~(1<<i);
        }
        
        if(flagUp)
            flag &= 0b10111111;
    }
}
