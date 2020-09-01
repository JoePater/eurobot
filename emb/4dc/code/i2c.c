
// PIC16F1773 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection Bits (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = ON        // Internal/External Switchover Mode (Internal/External Switchover Mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PPS1WAY = ON     // Peripheral Pin Select one-way control (The PPSLOCK bit cannot be cleared once it is set by software)
#pragma config ZCD = OFF        // Zero-cross detect disable (Zero-cross detect circuit is disabled at POR)
#pragma config PLLEN = ON       // Phase Lock Loop enable (4x PLL is always enabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = OFF      // Low-Power Brown Out Reset (Low-Power BOR is disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

#include "constants.h"
#include "i2c.h"

#define REG_INITIALISER {0,0,0,0,0,0,0,0,0,0};

const char i2c_slave_addr = 0x29;

char current_reg = 0;
char first_write = 0;

unsigned char i2c_registers[REG_NUMBER] = REG_INITIALISER;
unsigned char writeable_reg[REG_NUMBER] = {
     0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};

static void handle_read();
static void handle_write();
static void write_register(u8 reg,u8 val);
static u8 read_register(u8 reg);

/* RC2=clock, RC3=data */
void config_i2c()
{
     TRISC |= (0b1100);
     LATC &= ~0b1100;
     ANSELC = 0;
     WPUC = 0b1100;
     
     PIE1bits.SSP1IE = 1;
     SSP1ADD = i2c_slave_addr << 1;

     SSP1CON1 = 0b00110110; /* enable, mode */
     SSP1CON2 = 0b00000000; /* general call, clk stretch */
     SSP1CON3 = 0b00001010; /* interrupts, clk holding */

     SSPCLKPPS = 0b00010010; /* RC2 */
     SSPDATPPS = 0b00010011; /* RC3 */
     RC2PPS = 0b100001; /* clock */
     RC3PPS = 0b100010; /* data */
}

void i2c_isr()
{
     PIR1bits.SSP1IF = 0;
     if(SSP1STATbits.R_nW) handle_read();
     else handle_write();
     SSP1CONbits.CKP = 1;
}

static void handle_read()
{
     u8 x = SSP1BUF;
     u8 buf = read_register(current_reg);
     SSP1BUF = buf;
     //SSP1CONbits.CKP = 1; /* release clock */
}

static void handle_write()
{
     u8 buf = SSP1BUF;
     
     if(!SSP1STATbits.D_nA){
	  first_write = 1;
	  return;
     }
     if(first_write){
	  current_reg = buf;
	  first_write = 0;
     }else{
	  write_register(current_reg++,buf);
     }
}

static void write_register(u8 reg,u8 val)
{
     if(reg >= REG_NUMBER) return;
     u8 msk = writeable_reg[reg];
     /* mask out read-only bits, then get mask of read-only bits
	in the actual register and set those bits of the value */
     val &= msk;
     val |= (i2c_registers[reg] & ~msk);
	i2c_registers[reg] = val;
}

static u8 read_register(u8 reg)
{
     if(reg >= REG_NUMBER) return 0;
     return i2c_registers[reg];
}