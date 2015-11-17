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
    // Loop through and transmit entire string
    while (*data) { 
        usart_transmit(*data);
        data++;
    }
}

int uart_getchar(FILE *stream)
{
    
    char data = usart_receive(); //Temporarily store received data
    if(data == '\r')
        data = '\n';    
    stream_printf(data, stream); //Send to console what has been received, so we can see when typing
    return data;        
}

int USART0ReceiveByte(FILE *stream)
{
    uint8_t u8Data;
    // Wait for byte to be received
    while(!(UCSR0A&(1<<RXC0))){};

    u8Data=UDR0;
    // stream_printf(u8Data, stream); //Send to console what has been received, so we can see when typing
    return u8Data;
}

// Function called by printf to send to created stream
int stream_printf(unsigned char var, FILE *stream)
{
    // Translate \n to \r for terminal
    if (var == '\n') 
    {
        stream_printf('\r', 0);
    }

    // Transmit data
    usart_transmit(var);
    
    return 0;
}

//set stream pointer
FILE usart0_str = FDEV_SETUP_STREAM(stream_printf, USART0ReceiveByte, _FDEV_SETUP_RW);