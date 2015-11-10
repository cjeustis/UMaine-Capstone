#ifndef MOTORS
#define MOTORS

#include <avr/io.h>

void init_motors(void);
void motor_on(int motor);
void motor_off(int motor);

#endif