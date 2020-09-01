#include "pwm.h"

static void set_motor_pps(u8 motor,u8 a_mode,u8 b_mode); 

/* 
Motors 1,2,3,4 = PWM3,4,9,5

PWM1,2,3,4 = RC4,5,6,7
A1,2,3,4 = RA0,2,4,7
B1,2,3,4 = RA1,3,5,6
*/
static void set_pwms_dc(u8 m3,u8 m4,u8 m9,u8 m5)
{
     PWM3DCH = m3 >> 2;
     PWM3DCL = m3 << 6;
 
     PWM4DCH = m4 >> 2;
     PWM4DCL = m4 << 6;
 
     PWM9DCH = m9 >> 2;
     PWM9DCL = m9 << 6;

     PWM5DCH = 0;
     PWM5DCL = m5;
     PWM5LDCONbits.LDA = 1;
}

void config_pwm()
{     
     /* Configure clock stuff */
     T2CLKCON = 0b0001; /* clock source = Fosc/4 */
     T2PR = 0x3F; /* period */
     T2CON = 0b10100000; /* 4 prescaler, 8-bit overflow=7.81kHz */
     /* Configure 10-bit PWMs */
     PWM3CON = 0x90; /* enable, polarity */
     PWM4CON = 0x90;
     PWM9CON = 0x90;
     /* Configure 16-bit PWM */
     PWM5CLKCON = 0b00100000; /* 16 prescaler, 8-bit overflow=7.81kHz */
     PWM5LDCON = 0b00000000;
     PWM5PRH = 0; /* period = 255 */
     PWM5PRL = 0xFF;
     PWM5PHH = 0;
     PWM5PHL = 0;
     PWM5CON = 0x90;
     /* Init duty cycles */
     set_pwms_dc(0,0,0,0);
     /* Set PWM pins */
     LATC &= 0x0F;
     /* Setup pps */
     set_motor_pps(1,0,0);
     set_motor_pps(2,0,0);
     set_motor_pps(3,0,0);
     set_motor_pps(4,0,0);
     
}

/*
Shorted - A high, B high
Clockwise - A high, B PWM with active low
Anticlockwise - A PWM with active low, B high
Open - A low, B low
*/
/* bits 2,3 = A, bits 0,1 = B
0=low,1=high,2=PWM with active low */
static u8 get_a_val(char mode)
{
     switch(mode){
     case 'S':
	  return 1;
     case 'C':
	  return 1;
     case 'A':
	  return 2;
     case 'O':
     default:
	  return 0;
     }
}

static u8 get_b_val(char mode)
{
     switch(mode){
     case 'S':
	  return 1;
     case 'C':
	  return 2;
     case 'A':
	  return 1;
     case 'O':
     default:
	  return 0;
     }
}

u8 pps_pwm_vals[4] = {0b011001,0b011010,0b011011,0b011101};
/* mode=0 is low,mode=1 is high,mode=2 is pwm */
static void set_motor_pps(u8 motor,u8 a_mode,u8 b_mode)
{
     u8 pps_a = a_mode==2 ? pps_pwm_vals[motor-1] : 0;
     u8 pps_b = b_mode==2 ? pps_pwm_vals[motor-1] : 0;
     u8 lat_a = a_mode ? 1 : 0;
     u8 lat_b = b_mode ? 1 : 0;
     u8 x;
     switch(motor){
     case 1:
	  RA0PPS = pps_a;
	  RA1PPS = pps_b;
       x = LATA;
	  x &= 0xFC;
	  x |= (lat_b<<1) + (lat_a<<0);
       LATA = x;
	  break;
     case 2:
	  RA2PPS = pps_a;
	  RA3PPS = pps_b;
       x = LATA;
	  x &= 0xF3;
	  x |= (lat_b<<3) + (lat_a<<2);
       LATA = x;
	  break;
     case 3:
	  RA4PPS = pps_a;
	  RA5PPS = pps_b;
       x = LATA;
	  x &= 0xCF;
	  x |= (lat_b<<5) + (lat_a<<4);
       LATA = x;
	  break;
     case 4:
	  RA7PPS = pps_a;
	  RA6PPS = pps_b;
       x = LATA;
	  x &= 0x3F;
	  x |= (lat_b<<6) + (lat_a<<7);
       LATA = x;
	  break;
     }
}

void set_pwm_mode(u8 motor,char mode)
{
     u8 a = get_a_val(mode);
     u8 b = get_b_val(mode);
     set_motor_pps(motor,a,b);
}

void set_pwm(u8 motor,u8 pwm)
{
     switch(motor){
     case 1:
	  PWM3DCH = pwm >> 2;
	  PWM3DCL = pwm << 6;
	  break;
     case 2:
	  PWM4DCH = pwm >> 2;
	  PWM4DCL = pwm << 6;
	  break;
     case 3:
	  PWM9DCH = pwm >> 2;
	  PWM9DCL = pwm << 6;
	  break;
     case 4:
	  PWM5DCL = pwm;
	  PWM5DCH = 0;
       PWM5LDCONbits.LDA = 1;
	  break;
     }
}

