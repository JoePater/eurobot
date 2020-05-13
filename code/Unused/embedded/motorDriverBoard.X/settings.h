/* 
 * File:   settings.h
 * Author: root
 *
 * Created on 23 October 2019, 07:42
 */
#include <xc.h>

#ifndef SETTINGS_H
#define	SETTINGS_H

void setPWM(unsigned char pin,unsigned char pwm);
void setStatus(unsigned char s,unsigned char v,int v2);
void setRoutine(unsigned char s);
void revRoutine(unsigned char s);
void setMotors(unsigned char motor,unsigned char dir,unsigned char v);
void setDrive(unsigned char dir,unsigned char v);
void setDrive2(unsigned char dir,unsigned char v,unsigned char v2);

#endif	/* SETTINGS_H */

