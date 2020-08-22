#include "i2c.h"
#include "rot.h"

extern char ticks[NUM_ROT_ENC];
char curr_re = 0;

const unsigned char i2c_slave_addr = 40;

static void handle_read();
static void handle_write();

void config_i2c()
{
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
     unsigned char buf = SSP1BUF;
     //if(!SSP1STATbits.D_nA){
       //SSP1CONbits.CKP = 1; /* release clock */
     //}else if(SSP1CON2bits.ACKSTAT){
       //SSP1CONbits.CKP = 1; /* release clock */
	  //return; /* return if last data wasn't acknowledged */
     //}
     buf = ticks[curr_re];
     ticks[curr_re] = 0;
     curr_re = curr_re+1;
     if(curr_re >= NUM_ROT_ENC)curr_re = 0;
     SSP1BUF = buf;
     SSP1CONbits.CKP = 1; /* release clock */
}

static void handle_write()
{
     unsigned char buf = SSP1BUF;
     if(!SSP1STATbits.D_nA){
	  return;
     }
     curr_re = buf < NUM_ROT_ENC ? buf : 0;
}
