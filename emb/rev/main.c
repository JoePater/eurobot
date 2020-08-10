#include "i2c.h"
#include "servo.h"

void __interrupt isr()
{
     if(PIR1bits.CCP1IF){
	  PIR1bits.CCP1IF = 0;
	  i2c_isr();
     }
     else if(PIR1bits.CCP1IF){
	  PIR1bits.CCP1IF = 0;
	  servo_isr();
     }
}

int main()
{
     OSCCON = 0b11110010; /* 32MHz */
     INTCON = 0b11001000;

     config_servo();
     config_i2c();
     
     while(1);
     return 0;
}
