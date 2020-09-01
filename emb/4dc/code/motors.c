#include "motors.h"
#include "constants.h"
#include "pwm.h"
#include "i2c.h"

void set_motor(u8 motor,u8 dir,u8 pwm)			
{
     switch(dir){
     case 0: /* shorted */
	  set_pwm_mode(motor,'S');
	  set_pwm(motor,0);
	  break;
     case 1: /* clockwise */
	  set_pwm_mode(motor,'C');
	  set_pwm(motor,pwm);
	  break;
     case 2: /* anticlockwise */
	  set_pwm_mode(motor,'A');
	  set_pwm(motor,pwm);
	  break;
     case 3: /* open */
	  set_pwm_mode(motor,'O');
	  set_pwm(motor,0);
	  break;
     default:
          break;
     }
}

static void update_ctl()
{
     u8 c = i2c_registers[0];
     for(u8 i=1;i<=4;++i){
	  if(c & (0b1000<<i)){
	       set_motor(i,i2c_registers[(i<<1)-1],i2c_registers[(i<<1)]);
	  }else{
	       set_pwm_mode(i,'O');
	  }
     }
}

void update_motors()
{
     update_ctl();
     /*
     set_motor(1,i2c_registers[1],i2c_registers[2]);
     set_motor(2,i2c_registers[3],i2c_registers[4]);
     set_motor(3,i2c_registers[5],i2c_registers[6]);
     set_motor(4,i2c_registers[7],i2c_registers[8]);
      */
}
		    
