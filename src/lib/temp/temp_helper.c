#include "temp_helper.h"
#include <avr/io.h>

// Set temperature info
#define TEMP_LOWER 35
#define TEMP_UPPER 55
#define TEMP_OFFSET 230

struct TempSensors {
	int channel;
	float temp0F;
	float temp1F;
	float tempFinal;
	int user_defined_temp;
} temps;


void get_temp_reading(void) {
	// Start ADC0 conversion
	adc_enable_int();
	temps.channel = 0;
	start_adc(0);
	_delay_ms(10);

	// Start ADC1 conversion
	adc_enable_int();
	temps.channel = 1;
	_delay_ms(10);
	start_adc(1);
}

void update_temperature(void) {
	char value;
	printf("\n\nThe temperature is currently set to: %d\n", temps.user_defined_temp);
	printf("Enter a value between 35 F and 55 F that you would like the internal temperature to be maintaned at: ");

	while(1) {
		fgets(&value, 2, stdin);
		value += '0';
		if ((value >= TEMP_LOWER) && (value <= TEMP_UPPER)) {
			temps.user_defined_temp = value;
			break;
		}
		else {
			printf("\n\nInvalid value. Enter a value between 35 F and 55 F that you would like the internal temperature to be maintaned at: ");
		}
	}
	printf("\n\nThe desired internal temperature has been updated to %d F\n.", temps.user_defined_temp);
}

void set_temperature(void) {
	char choice;
	printf("\n\nMr. Pour: Temperature Control!\n--------------------\n\n");
	get_temp_reading();
	if (temps.tempFinal < 1) {
		get_temp_reading();
	}
	/* Display current Temperature */
	printf("Current temperature: %1.2f\n", temps.tempFinal);

	printf("\nWould you like to change the internal temperature? (y = yes, n = no): ");

	while(1) {
		scanf("%c", &choice);
		// fgets(&choice, 1, stdin);
		if (choice == 'y') {
			update_temperature();
			break;
		}
		else if (choice == 'n') {
			break;
		}
		else {
			printf("\n\nInvalid value. Would you like to change the internal temperature? (y = yes, n = no): ");
		}
	}
	welcome_screen();
}