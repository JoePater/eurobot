#include "us.h"

#define TIMER_PULSE_STOP 1
#define TIMER_RESET 500

unsigned char dist[NUM_US] = {0xFF,0xFF};

void config_us()
{
     TRISC = 0b1010;
     LATC = 0x00;
     ANSELC = 0x00;
     WPUC |= 0b1010;

     CCPR1L = TIMER_PULSE_STOP & 0xFF;
     CCPR1H = TIMER_PULSE_STOP >> 8;

     PIE1bits.CCP1IE = 1; /* enable interrupt */
     CCP1CON = 0b1010;

     TMR1L = 0;
     TMR1H = 0;
     /* 
	Fosc/4 source
	8 * prescaler = 1us increment ~ 6.5ms overflow
     */
     T1CON = 0b00110001;
}

static void set_pulse_pin(char us,char x)
{
     switch(us){
     case 0:
	  LATCbits.LATC0 = x;
	  break;
     case 1:
	  LATCbits.LATC2 = x;
	  break;
     }
}

static char get_echo_pin(char us)
{
     switch(us){
     case 0:
	  return PORTCbits.RC1;
     case 1:
	  return PORTCbits.RC3;
     }
     return 0;
}

static unsigned short get_timer()
{
     unsigned char high = TMR1H;
     unsigned char low = TMR1L;
     while(high != TMR1H){
	  high = TMR1H;
	  low = TMR1L;
     }
     return (high << 8) + low;
}

static void set_timer(unsigned short x)
{
     T1CON &= 0xFE;
     TMR1H = x >> 8;
     TMR1L = x & 0xFF;
     T1CON |= 0x01;
}

void us_isr()
{
     set_pulse_pin(0,0);
     set_pulse_pin(1,0);
}

char started[NUM_US];

void update_us(char us)
{
     char s = started[us];
     if(s && get_echo_pin(us)){
	  dist[us] = get_timer();
	  started[us] = 0;
     }
     if(get_timer() > TIMER_RESET){
	  set_timer(0);
	  started[us] = 1;
     }
}
