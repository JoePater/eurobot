#include "i2c.h"

extern char ticks[NUM_ROT_ENC];
char curr_re = 0;

const unsigned char i2c_slave_addr = 40;

void handle_read();
void handle_write();

void config_i2c()
{
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

void handle_read()
{
     if(!SSP1STATbits.D){
	  u8 buf = SSP1BUF;
     }else if(SSP1CON2bits.ACKSTAT){
	  return; /* return if last data wasn't acknowledged */
     }
     u8 buf = ticks[curr_re];
     ticks[curr_re] = 0;
     curr_re = curr_re+1;
     if(curr_re >= NUM_ROT_ENC)curr_re = 0;
     SSP1BUF = buf;
     SSP1CONbits.CKP = 1; /* release clock */
}

void handle_write()
{
     u8 buf = SSP1BUF;
     if(!SSP1STATbits.D){
	  return;
     }
     curr_re = buf < NUM_ROT_ENC ? buf : 0;
}
