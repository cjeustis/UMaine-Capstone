#include "adc.h"						// Prototypes

/* Convert the ADC voltage value into Fahrenheit */
float convert_adc_to_fahrenheit(uint16_t adc) {
	float c, v;

	v = adc * 5.0;
	v = v / 1024.0;						// Convert ADC value to voltage
	
	c = (v - 0.5) * 10.0;				// Convert voltage to Celcius

	return (c * 9.0 / 5.0) + 32.0;		// Convert Celcius to Fahrenheit and return value
}

/* Start ADC conversion (assuming interrupts are enabled) */
void start_adc(uint8_t channel) {
	channel &= 0b00000111;				// Set channel (& keeps it between 0-7)

	ADMUX = (ADMUX & 0xF8) | channel;	// Clear bottom 3 bits beforing setting channel

	ADCSRA |= (1<<ADSC);				// Start a Single conversion
}

/* Start ADC conversion (no interrupts enabled) and wait for conversion to compelete, returning ADC value) */
uint16_t start_adc_and_wait(uint8_t channel) {
	channel &= 0b00000111;				// Set channel (& keeps it between 0-7)

	ADMUX = (ADMUX & 0xF8) | channel;	// Clear bottom 3 bits beforing setting channel

	ADCSRA |= (1<<ADSC);				// Start a Single conversion

	while(ADCSRA & (1<<ADSC));			// Wait for conversion to complete

	return(ADC);						// Return ADC value
}

/* Disable ADC Conversion Complete interrupt */
void adc_disable_int(void) {
	ADCSRA &= ~(1<<ADIE);
}

/* Enable ADC Conversion Complete interrupt */
void adc_enable_int(void) {
	ADCSRA |= (1<<ADIE);
}

/* Takes integer as input and sets desired ADC reference voltage */
void set_reference(unsigned int ref) {
	ADMUX &= ~(1<<REFS1) | ~(1<<REFS0);			// Clear bits before setting (AREF, Internal Vref turned off)

	/* Set reference voltage */
	if (ref == 1)
	{
		ADMUX = (1<<REFS0);						// AVCC with ext. capacitor at AREF pin
	}
	else if (ref == 2)
	{
		ADMUX = (1<<REFS1);						// Internal 1.1V Voltage Reference with ext. capacitor at AREF pin
	}
	else if (ref == 3)
	{
		ADMUX = (1<<REFS1) | (1<<REFS0);		// Internal 2.56V Voltage Reference with ext. capacitor at AREF pin
	}
}

/* Takes integer as input and sets ADC frequency prescaler */
void set_prescaler(unsigned int prescaler) {
	ADCSRA &= ~(1<<ADTS2) | ~(1<<ADTS1) | ~(1<<ADTS0);			// Clear bits before setting (2)

	if (prescaler == 2)
	{
		ADCSRA |= (1<<ADTS0);					// 2
	}
	else if (prescaler == 4)
	{
		ADCSRA |= (1<<ADTS1);					// 4
	}
	else if (prescaler == 8)
	{
		ADCSRA |= (1<<ADTS1) | (1<<ADTS0);		// 8
	}
	else if (prescaler == 16)
	{
		ADCSRA |= (1<<ADTS2);					// 16
	}
	else if (prescaler == 32)
	{
		ADCSRA |= (1<<ADTS2) | (1<<ADTS0);		// 32
	}
	else if (prescaler == 64)
	{
		ADCSRA |= (1<<ADTS1);					// 64
	}
	else if (prescaler == 128)
	{
		ADCSRA |= (1<<ADTS2) | (1<<ADTS1) | (1<<ADTS0);			// 128
	}
}

/* Initialize and enable the ADC */
void init_adc(unsigned int ref, unsigned int prescaler) {
	set_reference(ref);				// Set the reference voltage
	set_prescaler(prescaler);		// Set the prescaler

	ADCSRA |= (1<<ADEN);			// Enable the ADC for conversions
}