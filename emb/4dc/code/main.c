#include "motors.h"
#include "i2c.h"
#include "pwm.h"
#include "io.h"
#include "constants.h"
#include "rev.h"

#include <xc.h>

void __interrupt() isr()
{
     if(PIR1bits.SSP1IF){
	  PIR1bits.SSP1IF = 0;
	  i2c_isr();
     }
}

void config_cpu()
{
     OSCCON = 0b11110010;
     INTCON = 0;//0b11000000;
     OPTION_REG = 0;
}

int main()
{
     config_cpu();
     config_io();
     config_pwm();
     config_i2c();
     config_rev();
     
     while(1){
          update_motors();
          if(PIR1bits.SSP1IF){
               PIR1bits.SSP1IF = 0;
               i2c_isr();
          }
          update_rev();
     }
     return 0;
}
