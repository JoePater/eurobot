#include "i2c.h"
#include "rot.h"

void __interrupt isr()
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
     
     while(1){
	  update_us();
     }
     return 0;
}
