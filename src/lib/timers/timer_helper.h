#ifndef TIMER_HELPER
#define TIMER_HELPER
#endif

#include <avr/io.h>

void init_motors_timer(void);
void enable_motors_timer_interrupt(void);
void disable_motors_timer_interrupt(void);