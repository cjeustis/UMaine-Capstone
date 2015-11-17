#ifndef MAIN
#define MAIN
#endif

/* PROTOTYPES */
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
/* END PROTOTYPES */