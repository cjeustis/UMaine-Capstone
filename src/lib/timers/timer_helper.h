#ifndef TIMER_HELPER
#define TIMER_HELPER
#endif

void init_motors_timer(void);
void enable_motor_timer(int motor);
void enable_motors_timer_interrupt(void);
void disable_motors_timer_interrupt(void);