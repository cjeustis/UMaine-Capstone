#ifndef SERIAL_H
#define SERIAL_H

extern FILE mystdout;

unsigned char check_recieve(void);
void transmit_data(unsigned char *data);
int USART0ReceiveByte(FILE *stream);
int stream_printf(unsigned char var, FILE *stream);

#endif