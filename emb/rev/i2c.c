#include "i2c.h"

const unsigned char i2c_slave_addr = 41;

static void handle_read();
static void handle_write();

void config_i2c()
{
     RA4PPS = 0b10000;
     RA5PPS = 0b10001;

     SSPCLKPPS = 0b00100;
     SSPDATPPS = 0b00101;
     
     PIE1.SSP1IE = 1;

     SSP1ADD = i2c_slave_addr << 1;
     SSP1CON1 = 0b00110110; /* enable, mode */
     SSP1CON2 = 0b00000000; /* general call, clk stretch */
     SSP1CON3 = 0b00001000; /* interrupts, clk holding */
}

void i2c_isr()
{
     PIR1.SSP1IF = 0;
     if(SSP1STATbits.R) handle_read();
     else handle_write();
}

static void handle_read()
{
     if(!SSP1STATbits.D){
	  u8 buf = SSP1BUF;
     }else if(SSP1CON2bits.ACKSTAT){
	  return; /* return if last data wasn't acknowledged */
     }
     u8 buf = get_servos();
     SSP1BUF = buf;
     SSP1CONbits.CKP = 1; /* release clock */
}

static void handle_write()
{
     u8 buf = SSP1BUF;
     if(!SSP1STATbits.D){
	  return;
     }
     set_servos(buf);
}
