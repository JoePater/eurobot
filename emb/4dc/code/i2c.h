#ifndef I2C_H
#define I2C_H

#include "constants.h"
#include <xc.h>

extern unsigned char i2c_registers[REG_NUMBER];

void config_i2c();
void i2c_isr();

#endif
