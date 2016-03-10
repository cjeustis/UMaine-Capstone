/* Defines */
#define BAUD 		9600								// Set baud for serial communication
#define BAUDRATE 	((F_CPU) / (BAUD * 16UL) - 1)		// ----------------------------------

#define TRANSMIT_RATE 	1								// Set usart info
#define DATA_BITS 		8								// ---------------
#define STOP_BITS 		1	
#define PARITY_BITS 	0

#define REFERENCE 		1								// Set adc info
#define PRESCALER 		128								// -------------

#define RECIPE_TOTAL_LENGTH 	112						// Recipe information
#define RECIPE_NAME_LENGTH 		16						// -------------------
#define RECIPE_AMOUNT_LENGTH 	3
#define NUMBER_OF_RECIPES 		5
#define USER_NAME_LENGTH 		17
#define USER_AMOUNT_LENGTH 		4

#define TEMP_LOWER 		35								// Temperature info
#define TEMP_UPPER 		55								// -----------------
#define TEMP_OFFSET 	4.5

#define MOTOR1A_ON 		~(1<<PC0)						// Motor info
#define MOTOR1B_ON 		(1<<PC1)						// -----------
#define MOTOR2A_ON 		~(1<<PC2)
#define MOTOR2B_ON 		(1<<PC3)
#define MOTOR3A_ON 		~(1<<PC4)
#define MOTOR3B_ON 		(1<<PC5)
#define MOTOR4A_ON 		~(1<<PC6)
#define MOTOR4B_ON 		(1<<PC7)

#define MOTORS_PORT 	PORTC
#define MOTORS_PORT_DIR DDRC

#define TENTH_OUNCE 	277.5							// Pouring Measurements for liquid timing
#define QUARTER_OUNCE 	(2.5 * TENTH_OUNCE)
#define HALF_OUNCE 		(5.0 * TENTH_OUNCE)
#define OUNCE 		(10.0 * TENTH_OUNCE)

#define MOTOR1_OFFSET	0
#define MOTOR2_OFFSET	0
#define MOTOR3_OFFSET	0
#define MOTOR4_OFFSET	0


/* Prototypes */
void init_timer(void);
void begin_pouring(int motor);
char* clean_string(int size, char string[]);
void pour_recipe(int recipe);
void update_recipe_name(int recipe);
void update_recipe_glass(int recipe);
void update_recipe_ingredient(int recipe, int ingredient);
void manage_recipe(int recipe);
void display_recipes(void);
void set_temperature(void);
void welcome_screen(void);
void enable_timer_interrupt(void);
void disable_timer_interrupt(void);
float convert_adc_to_fahrenheit(uint16_t adc);
void get_temp_reading(void);
void update_temperature(void);