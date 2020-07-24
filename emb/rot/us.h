#ifndef "US_H"
#define "US_H"

#define NUM_US 2

extern u8 dist[NUM_US];

void us_isr();
void config_us();
void update_us();

#endif
