#include "temp_helper.h"

/* Struc to hold temperature information */
struct TempSensors {
	int channel;
	float temp0F;
	float temp1F;
	float tempFinal;
	int user_defined_temp;
	bool isCooling;
	bool temp_reached;
	bool is_within_range;
} temps;


double eval_temp(uint16_t adc) {
	double temp;
  	// Assuming a 10k Thermistor.  Calculation is actually: Resistance = (1024/ADC)
	double resistance = ((10240000/adc) - 10000);

	/******************************************************************/
	/* Utilizes the Steinhart-Hart Thermistor Equation:				*/
	/*    Temperature in Kelvin = 1 / {A + B[ln(R)] + C[ln(R)]^3}		*/
	/*    where A = 0.000875020, B = 0.000157594 and C = 0.00000068055	*/
	/******************************************************************/

	temp = log(resistance);
	temp = 1 / (0.001127587560 + (0.0002343528476 * temp) + (0.00000008698059497 * temp * temp * temp));
	temp = temp - 273.15;  // Convert Kelvin to Celsius
	temp = (temp * 9.0 / 5.0) + 32.0;
	return temp + TEMP_OFFSET;
}

/* With no interrupt, read temp sensors and get temperature of inside */
void get_internal_temp(void) {
	temps.channel = 0;											// Set ADC Channel to 0
	float adc1 = start_adc_and_wait(0);							// Read ADC value
	temps.temp0F = eval_temp(adc1);

	temps.channel = 1;											// Set ADC Channel to 1
	float adc2 = start_adc_and_wait(1);							// Read ADC value
	temps.temp1F = eval_temp(adc2);

	temps.tempFinal = (temps.temp0F + temps.temp1F) / 2.0;		// Get average between the two sensors
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
	char value[3];

	printf("\n\nThe temperature is currently set to: %d\n", temps.user_defined_temp);
	printf("Enter a value between 35 F and 55 F that you would like the internal temperature to be maintaned at: ");

    if ((!fgets(value, sizeof value, stdin))) {
		printf("\nUnexpected response. Please try again.");
    }
    else if ((isdigit(value[0])) && (isdigit(value[1]))) {

	// scanf("%3d", &value);
	// fgets(&value, 3, stdin);					// Read in user temp
	// value -= '0';								// Convert to integer value
		if (((int)value >= TEMP_LOWER) && ((int)value <= TEMP_UPPER)) {
			temps.user_defined_temp = (int)&value;		// Set new value, if within range
			printf("\n\nThe desired internal temperature has been updated to %d F.\n", temps.user_defined_temp);
		}
	}
	else {
		printf("\n\nInvalid value. Returning...\n");
	}
}

/* Check to see if the user wants to change the internal temperature value */
void set_temperature(void) {
	char choice[2];

	printf("\n\nMr. Pour: Temperature Control!\n--------------------\n\n");

	get_temp_reading();								// Get temp value

	printf("Current temperature: %1.2f\n", temps.tempFinal);
	printf("\nWould you like to change the internal temperature? (y = yes, n = no): ");

    if ((!fgets(choice, sizeof choice, stdin))) {
		printf("\nUnexpected response. Please try again.");
    }
    else if (isalpha(choice[0])) {

	// scanf("%1c", &choice);
		if (choice[0] == 'y') {
			update_temperature();					// Let user update temp
		}
		else if (choice[0] == 'n') {
			welcome_screen();
		}
	}
	else {
		printf("\n\nInvalid value. Returning...\n");
		set_temperature();							// Try again...
	}
}