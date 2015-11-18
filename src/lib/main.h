/* Defines */
#define BAUD 9600									// Set baud for serial communication
#define BAUDRATE ((F_CPU) / (BAUD * 16UL) - 1)		// ---------------------------------

#define TRANSMIT_RATE 1								// Set usart info
#define DATA_BITS 8									// --------------
#define STOP_BITS 1									//
#define PARITY_BITS 0								//

#define REFERENCE 0									// Set adc info
#define PRESCALER 128								// ------------

#define RECIPE_TOTAL_LENGTH 112						// Recipe information
#define RECIPE_NAME_LENGTH 16						// ------------------
#define RECIPE_AMOUNT_LENGTH 3						//
#define NUMBER_OF_RECIPES 5							//
#define USER_NAME_LENGTH 17							//
#define USER_AMOUNT_LENGTH 4						//

#define TEMP_LOWER 35								// Temperature info
#define TEMP_UPPER 55								// ----------------
#define TEMP_OFFSET 230								//

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