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
  	// Assuming a 10k Thermistor.  Calculation is (actually): Resistance = (1024/ADC)
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
	temps.channel = 0;
	float adc1 = start_adc_and_wait(0);
	temps.temp0F = eval_temp(adc1);

	temps.channel = 1;
	float adc2 = start_adc_and_wait(1);
	temps.temp1F = eval_temp(adc2);

	temps.tempFinal = (temps.temp0F + temps.temp1F) / 2.0;
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
	int value;

	printf("\n\nThe temperature is currently set to: %d\n", temps.user_defined_temp);

	while(1) {
		printf("Enter a value between 35 F and 55 F that you would like the internal temperature to be maintaned at: ");
		scanf("%2d", &value);
		if ((value >= TEMP_LOWER) && (value <= TEMP_UPPER)) {
			temps.user_defined_temp = value;
			printf("\n\nThe desired internal temperature has been updated to %d F.\n", temps.user_defined_temp);
			break;
		}
		else {
			printf("\n\nInvalid value.\n");
		}
	}
	welcome_screen();
}

/* Check to see if the user wants to change the internal temperature value */
void set_temperature(void) {
	char choice[1];

	printf("\n\nMr. Pour: Temperature Control!\n--------------------\n\n");

	get_temp_reading();
	get_temp_reading();

	printf("Current temperature: %1.2f\n", (double)temps.tempFinal);

	while(1) {
		printf("\nWould you like to change the internal temperature? (y = yes, n = no): ");
		fgets(choice, 2, stdin);
		if (choice[0] == 'y') {
			update_temperature();	
		}
		else if (choice[0] == 'n') {
			welcome_screen();
		}
		else {
			printf("\n\nInvalid value.\n");
		}
	}
}