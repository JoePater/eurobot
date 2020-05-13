/* 
 * File:   updates.h
 * Author: joe
 *
 * Created on 11 December 2019, 18:20
 */
#include <xc.h>
#include "system.h"

#ifndef UPDATES_H
#define	UPDATES_H

void updateSys(struct properties *p);
void updateRev(struct rev *r);
char getProgress(unsigned int t,unsigned int total,char curr);
void updateMotors(struct properties *p);
void updateMovement(struct properties *p);
void testButtons(struct properties *p);
void setProps(unsigned char l,unsigned char r,unsigned char c);
void updateTime(struct properties *p);

#endif	/* UPDATES_H */

