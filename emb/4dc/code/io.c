#include "io.h"

void config_io()
{
     TRISA = 0x00;
     TRISB = 0x00;
     TRISC = 0x00;

     WPUA = 0x00;
     WPUB = 0x00;
     WPUC = 0x0C; /* on for RC2,3=i2c */
     WPUE = 0x08;

     ANSELA = 0x00;
     ANSELB = 0x00;
     ANSELC = 0x00;
}
