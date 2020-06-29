#include "i2c.h"

#define NUM_ROT_ENC 2

char ticks[NUM_ROT_ENC];

char old_a[NUM_ROT_ENC];
char old_b[NUM_ROT_ENC];

int getNext(char a, char b, char isA, char isCW) 
{
    if(isA && isCW) return b;
    if(!isA && isCW) return !a;
    if(isA && !isCW) return !b;
    if(!isA && !isCW) return a;
}

void update(char re)
{
     char a = old_a[re];
     char b = old_b[re];
     if (getA(re)==getNext(a,b,1,1) && getB(re)==getNext(a,b,0,1))
         tick += 1;         
     if (getA(re)==getNext(a,b 1, 0) && getB(re)==getNext(a,b, 0, 0))
         tick -= 1;
     old_a[re] = getA(re);
     old_b[re] = getB(re);
}

void __interrupt isr()
{
     if(/* i2c flag */) i2c_isr();
     else if(/* ioc flag */){
	  update(0);
	  update(1);
     }
}

int main()
{
     //config stuff
     while(1);
     return 0;
}
