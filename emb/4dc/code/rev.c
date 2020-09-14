#include "rev.h"
#include "i2c.h"

const short int_time_delay = 1000;

void config_rev(){
     /* Set up IO, timer */
     LATB = 0;
     TRISB = 0b11001000;/* RB3=in */
     //INLVLB = 0b10;
     
     T3CON = 0b11110000; /* ~4kHz */
}

static short get_timer(){
     u8 h,l;
     do{
          h = TMR3H;
          l = TMR3L;
     }while(h != TMR3H);
     return (h<<8) + l;
}

static void reset_timer(){
     TMR3L = 0;
     TMR3H = 0;
     TMR3L = 0;
}

static void stop_timer(){
     T3CONbits.ON = 0;
}

static void start_timer(){
     T3CONbits.ON = 1;
}

u8 prev_steps = 0;
void update_rev(){
     u8 steps_buf = i2c_registers[9];
     
     if(PORTBbits.RB3){ /* interrupted */
          short t = get_timer();
          if(t >= int_time_delay){
               if(i2c_registers[9])i2c_registers[9] -= 1;
          }
          reset_timer();
     }
     
     
     if(!steps_buf && prev_steps){ /* if stppped, set pwm to 0, stop timer */
          i2c_registers[8] = 0;
          stop_timer();
     }
     else if(steps_buf > prev_steps){ /* if started */
          start_timer();
          reset_timer();
     }
     prev_steps = steps_buf;
}