#ifndef PWM_H
#define PWM_H

#include "constants.h"
#include <xc.h>

void config_pwm();
void set_pwm(u8 motor,u8 pwm);
void set_pwm_mode(u8 motor,char mode);

#endif
