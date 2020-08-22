#ifndef ROT_H
#define ROT_H

#include "xc.h"

#define NUM_ROT_ENC 2

extern char ticks[NUM_ROT_ENC];

void update_re(char re);
void config_rot();

#endif
