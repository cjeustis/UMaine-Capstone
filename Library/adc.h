#ifndef ADC
#define ADC

#include <avr/io.h>

uint16_t ReadADC(uint8_t channel);
void set_reference(unsigned int ref);
void set_prescaler(unsigned int prescaler);
void init_adc(unsigned int ref, unsigned int prescaler);

#endif