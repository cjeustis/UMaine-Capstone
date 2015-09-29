#include "serial.h"
#include <avr/io.h>
#include <stdio.h>

unsigned char check_recieve(void)
{
    // Return 1 if receive complete, else 0
    unsigned char received = 0;

    // Check if received data
    if (UCSR0A & (1<<RXC0))
    {
        received = 1;
    }

    return received;
}

void transmit_data(unsigned char *data)
{
    // Loop through and trasnmit entire string
    while (*data) { 
        usart_transmit(*data);
        data++;
    }
}
 
// Function called by printf to send to created stream
int stream_printf(unsigned char var, FILE *stream)
{
    // Translate \n to \r for terminal
    if (var == '\n') 
    {
        usart_transmit('\r');
    }

    // Transmit data
    usart_transmit(var);
    
    return 0;
}

// create stream pointing to serial
FILE mystdout = FDEV_SETUP_STREAM(stream_printf, NULL, _FDEV_SETUP_WRITE);