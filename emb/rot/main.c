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


#include "i2c.h"
#include "rot.h"
#include "us.h"

#include "xc.h"

void __interrupt() isr(void)
{
     if(PIR1bits.SSP1IF){
	  PIR1bits.SSP1IF = 0;
	  i2c_isr();
     }
     else if(IOCAF & 0x0F){
	  IOCAF = 0;
	  update_re(0);
	  update_re(1);
     }
     else if(PIR1bits.CCP1IF){
	  PIR1bits.CCP1IF = 0;
	  us_isr();
     }
}

int main()
{
     OSCCON = 0b11110010; /* 32MHz */
     INTCON = 0b11001000;
     OPTION_REG = 0;
     
     config_i2c();
     config_rot();
     config_us();
     
     while(1){
	  update_us(0);
       update_us(1);
     }
     return 0;
}
