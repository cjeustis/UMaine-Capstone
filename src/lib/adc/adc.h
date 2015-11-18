#ifndef ADC
#define ADC
#endif

float convert_adc_to_fahrenheit(uint16_t adc);
uint16_t ReadADC(uint8_t channel);
void set_reference(unsigned int ref);
void set_prescaler(unsigned int prescaler);
void init_adc(unsigned int ref, unsigned int prescaler);
void adc_disable_int(void);
void adc_enable_int(void);