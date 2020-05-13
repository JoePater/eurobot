/* 
 * File:   i2c.h
 * Author: joe
 *
 * Created on 11 December 2019, 12:43
 */
#include <xc.h>

#ifndef I2C_H
#define	I2C_H

/* bit 7 = open?
 * bit 6 = switch pressed?
 * bit 5 = waiting for cup?
 * 
 */
typedef unsigned char holder;
extern holder harr[5];

void i2cISR(void);
void checkData(unsigned char);
void handleWrite(unsigned char);
void handleRead(void);

#endif	/* I2C_H */

