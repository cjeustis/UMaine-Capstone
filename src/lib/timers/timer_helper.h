#ifndef TIMER_HELPER_H
#define TIMER_HELPER_H

void init_motors_timer(void);
void enable_motor_timer(int motor);
void enable_temp_sensor_timer_interrupt(void);
void disable_temp_sensor_timer_interrupt(void);
void enable_motors_timer_interrupt(void);
void disable_motors_timer_interrupt(void);

#endif