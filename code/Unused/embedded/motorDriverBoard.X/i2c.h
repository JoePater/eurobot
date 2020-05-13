/* 
 * File:   i2c.h
 * Author: joe
 *
 * Created on 11 December 2019, 12:43
 */
#include <xc.h>

#ifndef I2C_H
#define	I2C_H

void i2cISR(void);
void checkData(unsigned char,unsigned char,int);
void handleWrite(unsigned char);
void handleRead(void);

#endif	/* I2C_H */

