#ifndef SERIAL
#define SERIAL

#include <avr/io.h>
#include <stdio.h>

extern FILE mystdout;

unsigned char check_recieve(void);
void transmit_data(unsigned char *s);
int stream_printf(unsigned char var, FILE *stream);

#endif