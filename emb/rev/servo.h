#ifndef SERVO_H
#define SERVO_H

void config_servo();
void servo_isr();
void set_servos(unsigned char x);
unsigned char get_servos();

#endif