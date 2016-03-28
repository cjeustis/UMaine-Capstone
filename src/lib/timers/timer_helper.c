#include "timer_helper.h"				// Prototypes

#define TEN_SECONDS 1220				// Number of overflows on Timer3 for 10 seconds 

/* Timer counters and pouring length variables */
volatile float motors_overflow_count;
volatile int temp_overflow_count;
static float pouring_length;
static int percent;

/* Initializes both Timer/Counter1 and Timer/Counter2
   - Timer/Counter1 is used for controlling how long motors remain on for dispensing liquids
   - Timer/Counter2 is used to check internal temperature every so often to ensure internal temperature is being maintained
*/
void init_motors_timer(void)
{
	/* Normal operation of timers */
	TCCR1A = 0x00;						// Timer/Counter1
	TCCR3A = 0x00;						// Timer/Counter3

	/* Prescaler = 8 */
	TCCR1B = (1<<CS10);					// Timer/Counter1
	TCCR3B = (1<<CS10);					// Timer/Counter3
	
	/* Initialize counter to 0 */
	TCNT1 = 0;							// Timer/Counter1
	TCNT3 = 0;							// Timer/Counter3
	
	/* No forced output compares */
	TCCR1C = 0x00;						// Timer/Counter1
	TCCR3C = 0x00;						// Timer/Counter3

	motors_overflow_count = 0.0;		// Initialize motor timer overflow count to 0
	temp_overflow_count = 0;			// Initialize temp sensor reading overflow count to 0
}

/* Enables the given motor to start dispensing liquid - sets time and waits unil complete */
void enable_motor_timer(int motor)
{
	char lcd_string[17];
	char lcd_string1[17];
	int temp;
	if(pouring_length != 0) {
		// LCDClear();
		motor_on(motor);
		enable_motors_timer_interrupt();

		while (motors_overflow_count < pouring_length) {	
			temp = (motors_overflow_count / pouring_length) * 100;
			if (percent != temp) {
				percent = (motors_overflow_count / pouring_length) * 100;
				sprintf(lcd_string, "Complete:   %d%%", percent);
				sprintf(lcd_string1, "Pouring # %d", motor);
				// LCDWriteString(lcd_string1);
				// LCDWriteStringXY(0, 1, lcd_string);
				// printf("\rPouring: %d%%", percent);
				// fflush(stdout);
			}
		}
		motor_off(motor);
		disable_motors_timer_interrupt();
		motors_overflow_count = 0.0;

		// LCDClear();
		// LCDWriteString("Pouring recipe..");
		// LCDWriteStringXY(0, 1, "Complete:   100%%");

		// printf("\rPouring: 100%%\n");
		// fflush(stdout);
	}
}

/* Enable the Timer/Counter3 Overflow interrupt */
void enable_temp_sensor_timer_interrupt(void)
{
	TIMSK3 = (1<<TOIE3);
}

/* Disable the Timer/Counter3 Overflow interrupt */
void disable_temp_sensor_timer_interrupt(void)
{
	TIMSK1 &= ~(1<<TOIE1);
}

/* Enable the Timer/Counter1 Overflow interrupt */
void enable_motors_timer_interrupt(void)
{
	TIMSK1 = (1<<TOIE1);
}

/* Disable the Timer/Counter1 Overflow interrupt */
void disable_motors_timer_interrupt(void)
{
	TIMSK1 &= ~(1<<TOIE1);
}