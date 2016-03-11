#include "serial.h"

/* Check if there has been anything recieved over serial port */
unsigned char check_recieve(void) {
    unsigned char received = 0;                 // Return 1 if receive complete, else 0

    if (UCSR0A & (1<<RXC0)) {                   // Check if received data
        received = 1;
    }

    return received;
}

/* Transmit the given data over the serial port */
void transmit_data(unsigned char *data) {
    while (*data) {                             // Loop through and transmit entire string
        usart_transmit(*data);
        data++;
    }
}

/* Get character from stream */
int uart_getchar(FILE *stream) {
    char data = usart_receive();                // Temporarily store the received data
    if(data == '\r') {
        data = '\n';                            // Convert for terminal to understand
    }
    // stream_printf(data, stream);                // Send to console what has been received, so we can see when typing
    return data;        
}

/* Read data from the serial port */
int USART0ReceiveByte(FILE *stream) {
    while(!(UCSR0A&(1<<RXC0))){};               // Wait for byte to be received
    unsigned char data = UDR0;

    // stream_printf(data, stream);              // Send to console what has been received, so we can see when typing
    return data;
}

/* Function called by printf to send data to created stream */
int stream_printf(unsigned char var, FILE *stream) {
    if (var == '\n') 
    {
        stream_printf('\r', 0);                 // Translate \n to \r for terminal
    }

    usart_transmit(var);                        // Transmit data
    
    return 0;
}

/* Set stream pointer to translate stdin/stdout to serial port */
FILE usart0_str = FDEV_SETUP_STREAM(stream_printf, USART0ReceiveByte, _FDEV_SETUP_RW);