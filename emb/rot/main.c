#include "i2c.h"
#include "rot.h"
#include "us.h"

#include "xc.h"

void __interrupt() isr(void)
{
     if(PIR1bits.CCP1IF){
	  PIR1bits.CCP1IF = 0;
	  i2c_isr();
     }
     else if(IOCAF & 0x0F){
	  IOCAF = 0;
	  update_re(0);
	  update_re(1);
     }
     else if(PIR1bits.CCP1IF){
	  PIR1bits.CCP1IF = 0;
	  us_isr();
     }
}

int main()
{
     OSCCON = 0b11110010; /* 32MHz */
     INTCON = 0b11001000;
     
     config_i2c();
     config_rot();
     config_us();
     
     while(1){
	  update_us(0);
      update_us(1);
     }
     return 0;
}
