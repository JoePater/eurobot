#ifndef US_H
#define US_H

#include "xc.h"

#define NUM_US 2

extern unsigned char dist[NUM_US];

void us_isr();
void config_us();
void update_us(char us);

#endif
