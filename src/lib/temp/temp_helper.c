#include "temp_helper.h"

/* Struc to hold temperature information */
struct TempSensors {
	int channel;
	float temp0F;
	float temp1F;
	float tempFinal;
	int user_defined_temp;
} temps;

/* With no interrupt, read temp sensors and get temperature of inside */
void get_internal_temp(void) {
	temps.channel = 0;												// Set ADC Channel to 0
	float temp1 = start_adc_and_wait(0);							// Read ADC value
	temps.temp0F = convert_adc_to_fahrenheit(temp1+TEMP_OFFSET);	// Convert to Fahrenheit

	temps.channel = 1;												// Set ADC Channel to 1
	float temp2 = start_adc_and_wait(1);							// Read ADC value
	temps.temp1F = convert_adc_to_fahrenheit(temp2+TEMP_OFFSET);	// Convert to Fahrenheit

	temps.tempFinal = (temps.temp0F + temps.temp1F) / 2.0;			// Get average between the two sensors
}

/* Use ADC interrupt to read temp sensors */
void get_temp_reading(void) {
	adc_enable_int();			// Enable ADC Conversion Complete interrupt
	temps.channel = 0;			// Set ADC Channel to 0
	start_adc(0);				// Start ADC and read temp
	_delay_ms(10);				// Need delay (probably not this much) so timing is okay

	// Start ADC1 conversion
	adc_enable_int();			// Enable ADC Conversion Complete interrupt
	temps.channel = 1;			// Set ADC Channel to 1
	_delay_ms(10);				// Need delay (probably not this much) so timing is okay
	start_adc(1);				// Start ADC and read temp
}

/* Let user change desired internal temperature */
void update_temperature(void) {
	char value;

	printf("\n\nThe temperature is currently set to: %d\n", temps.user_defined_temp);
	printf("Enter a value between 35 F and 55 F that you would like the internal temperature to be maintaned at: ");

	while(1) {
		fgets(&value, 3, stdin);					// Read in user temp
		value -= '0';								// Convert to integer value
		if ((value >= TEMP_LOWER) && (value <= TEMP_UPPER)) {
			temps.user_defined_temp = value;		// Set new value, if within range
			break;
		}
		else {
			printf("\n\nInvalid value. Enter a value between 35 F and 55 F that you would like the internal temperature to be maintaned at: ");
		}
	}
	printf("\n\nThe desired internal temperature has been updated to %d F\n.", temps.user_defined_temp);
}

/* Check to see if the user wants to change the internal temperature value */
void set_temperature(void) {
	char choice;

	printf("\n\nMr. Pour: Temperature Control!\n--------------------\n\n");

	get_temp_reading();								// Get temp value

	printf("Current temperature: %1.2f\n", temps.tempFinal);
	printf("\nWould you like to change the internal temperature? (y = yes, n = no): ");

	while(1) {
		scanf("%c", &choice);
		if (choice == 'y') {
			update_temperature();					// Let user update temp
			break;
		}
		else if (choice == 'n') {
			break;
		}
		else {
			printf("\n\nInvalid value. Would you like to change the internal temperature? (y = yes, n = no): ");
		}
	}
	welcome_screen();								// Go back to start of program
}