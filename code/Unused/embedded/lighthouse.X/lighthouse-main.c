// PIC16F1703 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection Bits (Internal HFINTOSC. I/O function on RA4 and RA5.)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select (MCLR/VPP pin function is digital input)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PPS1WAY = ON     // Peripheral Pin Select one-way control (The PPSLOCK bit cannot be cleared once it is set by software)
#pragma config ZCDDIS = ON      // Zero-cross detect disable (Zero-cross detect circuit is disabled at POR)
#pragma config PLLEN = ON       // Phase Lock Loop enable (4x PLL is always enabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = OFF      // Low-Power Brown Out Reset (Low-Power BOR is disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>


void main(void){
    OSCCON = 0b00000010;
    OPTION_REG = 0b00001000;
    
    TRISA = 0b001100; //RA0=top motor,RA1=pulley motor,RA2=button,RA3=stop sensor
    PORTA = 0b000000;
    ANSELA = 0b000000;
    WPUA = 0b001100;
    
    
    char on = 0;
    char finished = 0;
    while(1){
        if(PORTAbits.RA2 == 0 && !on){
            on = 1;
            PORTAbits.RA0 = 1;
            PORTAbits.RA1 = 1;
        }
        if(!finished && on && PORTAbits.RA3==0){
            PORTAbits.RA1 = 0;
            finished = 1;
        }
    }
}
