#include "rot.h"

char ticks[NUM_ROT_ENC];

char old_a[NUM_ROT_ENC];
char old_b[NUM_ROT_ENC];

void config_rot()
{
     TRISA = 0x0F;
     LATA = 0x00;
     ANSELA = 0x00;
     WPUA = 0b00111111;

     IOCAP = 0x0F;
     IOCAN = 0x0F;
}

static char getA(char re)
{
     switch(re){
     case 0:
	  return !PORTAbits.RA0;
     case 1:
	  return !PORTAbits.RA2;
     }
     return 0;
}

static char getB(char re)
{
     switch(re){
     case 0:
	  return !PORTAbits.RA1;
     case 1:
	  return !PORTAbits.RA3;
     }
     return 0;
}

static int getNext(char a, char b, char isA, char isCW) 
{
    if(isA && isCW) return b;
    if(!isA && isCW) return !a;
    if(isA && !isCW) return !b;
    if(!isA && !isCW) return a;
    return 0;
}

void update_re(char re)
{
     char a = old_a[re];
     char b = old_b[re];
     char na = getA(re);
     char nb = getB(re);
     if (na==getNext(a,b,1,1) && nb==getNext(a,b,0,1))
         ticks[re] += 1;         
     if (na==getNext(a,b,1, 0) && nb==getNext(a,b, 0, 0))
         ticks[re] -= 1;
     old_a[re] = na;
     old_b[re] = nb;
}
