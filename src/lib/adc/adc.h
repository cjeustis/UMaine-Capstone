#ifndef ADC_H
#define ADC_H

float convert_adc_to_fahrenheit(uint16_t adc);
void start_adc(uint8_t channel);
uint16_t start_adc_and_wait(uint8_t channel);
void set_reference(unsigned int ref);
void set_prescaler(unsigned int prescaler);
void init_adc(unsigned int ref, unsigned int prescaler);
void adc_disable_int(void);
void adc_enable_int(void);

#endif