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
     u8 high = TMR1H;
     u8 low = TMR1L
     while(high != TMR1H){
	  high = TMR1H;
	  low = TMR1L;
     }
     return high << 8 + low;
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

}

void config_servo()
{

}

void servo_isr()
{
     //clear flag
     
     for(char i=0;i<NUM_SERVOS;++i){
	  if(servos[i].ticks[servos[i].stat] > get_timer1()){
	       LATC &= ~(1 << servos[i].latc_bit);
	  }
     }
     if(++time_ind >= NUM_INT_TIMERS){
	  //reset index,load stat buffer,clear timer,set pins
	  time_ind = 0;
	  for(char i=0;i<NUM_SERVOS;++i){
	       servos[i].stat = ((stat_buf >> i) & 1);
	  }
	  set_timer1(0);
	  LATC |= 0b111111;
     }
     set_int_ticks(interrupt_timers[time_ind]);
}

void set_servos(unsigned char x)
{
     stat_buf = x;
}
     
unsigned char get_servos()
{
     return stat_buf;
}
