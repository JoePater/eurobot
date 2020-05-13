/* 
 * File:   configs.h
 * Author: root
 *
 * Created on 21 October 2019, 09:17
 */
#include <xc.h>

#ifndef CONFIGS_H
#define	CONFIGS_H

//#define SINE_REQUIRED
#define I2C_REQUIRED

void configureCPU(void);
void configureIOs(void);
void configureTimers(void);
void configureADC(void);
void configureSMT(void);
void configurePWM(void);
#ifdef I2C_REQUIRED
void configureI2C(void);
#endif

#endif	/* CONFIGS_H */

