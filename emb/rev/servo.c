#include "servo.h"

#define NUM_SERVOS 6 /* ignore 5th */

#define SERVO_INIT(NUM,TICKS0,TICKS1,BIT) {.num=NUM,.stat=0,.ticks={TICKS0,TICKS1},.latc_bit=BIT}

struct servo {
     unsigned char num; /* servo number */
     unsigned char stat; /* either on (1) or off (0) */
     unsigned short ticks[2];   /* time to turn off when off/on */
     unsigned char latc_bit; /* bit number in LATC */
};

struct servo servos[NUM_SERVOS] = {
     SERVO_INIT(0,1000,2000,0),
     SERVO_INIT(1,1000,2000,1),
     SERVO_INIT(2,1000,2000,2),
     SERVO_INIT(3,1000,2000,3),
     SERVO_INIT(4,1000,2000,4),
     SERVO_INIT(5,2000,3000,5)
};

unsigned char stat_buf = 0;

#define NUM_INT_TIMERS 4

unsigned short interrupt_times[NUM_INT_TIMERS] = {1000,2000,3000,40000};
char time_ind = 0;

static unsigned short get_timer1()
{
     unsigned char high = TMR1H;
     unsigned char low = TMR1L;
     while(high != TMR1H){
	  high = TMR1H;
	  low = TMR1L;
     }
     return (high << 8) + low;
}

static void set_timer1(unsigned short x)
{
     T1CON &= 0xFE;
     TMR1H = x >> 8;
     TMR1L = x & 0xFF;
     T1CON |= 0x01;
}

static void set_int_ticks(unsigned short ticks)
{
     CCPR1H = ticks >> 8;
     CCPR1L = ticks & 0xFF;
}

void config_servo()
{
     TRISC = 0;
     WPUC = 0;
     ANSELC = 0;
     LATC = 0;
     
     set_int_ticks(interrupt_times[time_ind]);
     CCP1CON = 0b1010; /* Compare mode with interrupt */
     set_timer1(0);
     T1CON = 0b00100001; /* 4* prescaler */
     PIE1bits.CCP1IE = 1;
}

void servo_isr()
{
     if(servos[0].ticks[servos[0].stat] < get_timer1()){
	       LATC &= ~(1 << servos[0].latc_bit);
	}
     if(servos[1].ticks[servos[1].stat] < get_timer1()){
	       LATC &= ~(1 << servos[1].latc_bit);
	}
     if(servos[2].ticks[servos[2].stat] < get_timer1()){
	       LATC &= ~(1 << servos[2].latc_bit);
	}
     if(servos[3].ticks[servos[3].stat] < get_timer1()){
	       LATC &= ~(1 << servos[3].latc_bit);
	}
     if(++time_ind >= NUM_INT_TIMERS){
	  //reset index,load stat buffer,clear timer,set pins
	  time_ind = 0;
	  
	  servos[0].stat = ((stat_buf >> 0) & 1);
       servos[1].stat = ((stat_buf >> 1) & 1);
       servos[2].stat = ((stat_buf >> 2) & 1);
       servos[3].stat = ((stat_buf >> 3) & 1);
       
	  set_timer1(0);
	  LATC |= 0b111111;
     }
     set_int_ticks(interrupt_times[time_ind]);
}

void set_servos(unsigned char x)
{
     stat_buf = x;
}
     
unsigned char get_servos()
{
     return stat_buf;
}
