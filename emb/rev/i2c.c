#include "i2c.h"
#include "servo.h"

const unsigned char i2c_slave_addr = 41;

static void handle_read();
static void handle_write();

void config_i2c()
{
     TRISA = 0x3F;
     LATA = 0x00;
     ANSELA = 0x00;
     WPUA = 0b00111111;
     
     RA4PPS = 0b10000;
     RA5PPS = 0b10001;

     SSPCLKPPS = 0b00100;
     SSPDATPPS = 0b00101;
     
     PIE1 |= 1 << 3;

     SSP1ADD = i2c_slave_addr << 1;
     SSP1CON1 = 0b00110110; /* enable, mode */
     SSP1CON2 = 0b00000000; /* general call, clk stretch */
     SSP1CON3 = 0b00001000; /* interrupts, clk holding */
}

void i2c_isr()
{
     PIR1 &= ~(1 << 3);
     if(SSP1STATbits.R_nW) handle_read();
     else handle_write();
}

static void handle_read()
{
     unsigned char buf = get_servos();
     SSP1BUF = buf;
     SSP1CONbits.CKP = 1; /* release clock */
}

static void handle_write()
{
     unsigned char buf = SSP1BUF;
     if(!SSP1STATbits.D_nA){
	  return;
     }
     set_servos(buf);
}
