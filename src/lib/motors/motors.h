#ifndef MOTORS_H
#define MOTORS_H

#define MOTOR1A_ON 	~(1<<PC0)
#define MOTOR1B_ON 	(1<<PC1)
#define MOTOR2A_ON 	~(1<<PC2)
#define MOTOR2B_ON 	(1<<PC3)
#define MOTOR3A_ON 	~(1<<PC4)
#define MOTOR3B_ON 	(1<<PC5)
#define MOTOR4A_ON 	~(1<<PC6)
#define MOTOR4B_ON	 	(1<<PC7)

#define MOTORS_PORT 	PORTC
#define MOTORS_PORT_DIR 	DDRC

#define MOTOR1_OFFSET	0
#define MOTOR2_OFFSET	0
#define MOTOR3_OFFSET	0
#define MOTOR4_OFFSET	0

void init_motors(void);
void motor_on(int motor);
void motor_off(int motor);

#endif