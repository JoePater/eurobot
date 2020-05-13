#include "configs.h"
#define I2C_REQUIRED



void configureCPU(void){
    OSCCON1 = 0b01100000;
    OSCFRQ = 0b110; //32MHz
    
    INTCON = 0b11000000; //Interrupts enabled
    PIE3bits.SSP1IE = 1;
}

void configureIOs(void){
    LATA = 0;
    LATB = 0;
    LATC = 0;
    LATD = 0;
    LATE = 0;
    
    INLVLA = 0;
    INLVLB = 0;
    INLVLE = 0;
    
    
    TRISA = 0b11000000; //A6+7 = claw up/down
    TRISB = 0b11001111; //B0,1,2,3 = claw clamped,release,right way up,upside down
    TRISC = 0b00000000;
    TRISD = 0b00000000;
    TRISE = 0b0011; //E1 = rev sensor
    
    WPUA = 0b11000000;
    WPUB = 0b11001111;
    WPUC = 0;
    WPUD = 0;
    WPUE = 0b0111;
    
    ANSELA = 0;
    ANSELB = 0;
    ANSELC = 0;
    ANSELD = 0;
    ANSELE = 1; //e0 is motor current
}

void configureADC(void){
    FVRCON = 0b10000001; //1.024V
    
    ADCON0 = 0b11000100; //Right justified
    ADCON1 = 0b00000000;
    ADCON2 = 0b10110010;
    ADCON3 = 0b00100000;
    ADCLK = 0b00111111;
    ADREF = 0b00000011;
    ADPCH = 0b00100000;
    ADPRE = 128; //4uS
    ADACQ = 128; //4uS
    ADCNT = 0;
    
    
}

void configureTimers(void){
    T0CON0 = 0b10010000;
	T0CON1 = 0b01001011; //1:2048 prescaler, 1kHz
	
#ifdef SINE_REQUIRED	
	T1CON = 0b00000011;
	T1GCON = 0b00000000;
	T1CLK = 0b00000001; //no prescaler, 8MHz, sine wave
#endif
	
	T2CLKCON = 0b00000001; //pwm, Fosc/4
	T2CON = 0b00000000;
	
	T3CON = 0b00000001;
	T3GCON = 0b00000000;
	T3CLK = 0b00000001; //no prescaler, 8MHz, small delays
}

#ifdef I2C_REQUIRED
void configureI2C(void){
	SSP1STAT = 0b01000000;
    SSP1CON1 = 0b00100110;
    SSP1CON2 = 0b00000001;
    SSP1CON3 = 0b00101011;
    SSP1ADD = 0b00110010; //address = 0b0011001
    
    SSP1CLKPPS = 0x0E; //RB6
    SSP1DATPPS = 0x0F; //RB7
    RB6PPS = 0x14;
    RB7PPS = 0x15;
}
#endif

void configureSMT(void){
    
}

void configurePWM(void){
    PR2 = 0xFF;
    
    CCP1CON = 0b10011111; //left aligned
    CCP2CON = 0b10011111;
    CCP3CON = 0b10011111;
    CCP4CON = 0b10011111;
    CCP5CON = 0b10011111;
    PWM6CON = 0b00000000;
    PWM7CON = 0b00000000;
    
    CCPR1L = 0;
    CCPR2L = 0;
    CCPR3L = 0;
    CCPR4L = 0;
    CCPR5L = 0;
    PWM6DCL = 0;
    PWM7DCL = 0;
    
    CCPR1H = 0;
    CCPR2H = 0;
    CCPR3H = 0;
    CCPR4H = 0;
    CCPR5H = 0;
    PWM6DCH = 0;
    PWM7DCH = 250; //used for on (input to CWG)
    
    CWG3CON0 = 0b00000100;
    CWG3CON1 = 0b00000000;
    CWG3DBR = 0x10; //0.5uS dead time
    CWG3DBF = 0x10;
    CWG3CLKCON = 0; //Fosc
    CWG3ISM = 0b0111; //PWM7
    CWG3CON0bits.EN = 1;
    CWG3CON0bits.LD = 1;
    
    CWG1CON0 = 0b00000100;
    CWG1CON1 = 0b00000000;
    CWG1DBR = 0x10; //0.5uS dead time
    CWG1DBF = 0x10;
    CWG1CLKCON = 0; //Fosc
    CWG1ISM = 0b0111; //PWM7
    CWG1CON0bits.EN = 1;
    CWG1CON0bits.LD = 1;
    
    CLC1CON = 0b00000000; //AND-OR
    CLC1SEL0 = 0b00101010; //CWG1A
    CLC1SEL1 = 0b00101010; //CWG1A
    CLC1POL = 0b00000000;
    CLC1GLS0 = 0b00000010;
    CLC1GLS1 = 0b00001000;
    CLC1GLS2 = 0;
    CLC1GLS3 = 0;
    CLC1CONbits.LC1EN = 1;
    
    CLC2CON = 0b00000000; //AND-OR
    CLC2SEL0 = 0b00101011; //CWG1B
    CLC2SEL1 = 0b00101011; //CWG1B
    CLC2POL = 0b00000000;
    CLC2GLS0 = 0b00000010;
    CLC2GLS1 = 0b00001000;
    CLC2GLS2 = 0;
    CLC2GLS3 = 0;
    CLC2CONbits.LC2EN = 1;
    
    PIR4bits.TMR2IF = 0;
    T2CONbits.ON = 1;
    PWM6CONbits.EN = 1;
    PWM7CONbits.EN = 1;
}




