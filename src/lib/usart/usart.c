#include "usart.h"

/* Set the transmit rate of USART communication */
void set_transmit_rate(unsigned int transmit) {
    UCSR0A &= ~(1<<U2X0);                   // Clear bit before starting

    if (transmit == 1)
    {
        UCSR0A = (1<<U2X0);                 // Double transmission rate (good/needed for running at 16Mhz clock speed)
    }
}

/* Set frame size - how many bits */
void set_frame_size(unsigned int bits) {
    UCSR0C &= ~(1<<UCSZ02) | ~(1<<UCSZ01) | ~(1<<UCSZ00);   // Clear bits before starting

    if (bits == 6)
    {
        UCSR0C |= (1<<UCSZ00);                              // 6 bits
    }
    else if (bits == 7)
    {
        UCSR0C |= (1<<UCSZ01);                              // 7 bits
    }
    else if (bits == 8)
    {
        UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);                // 8 bits
    }
    else if (bits == 9)
    {
        UCSR0C |= (1<<UCSZ02) | (1<<UCSZ01) | (1<<UCSZ00);  // 9 bits
    }
}

/* Set how many stop bits to use */
void set_stop_bits(unsigned int bits) {
    UCSR0C &= ~(1<<USBS0);                                  // Clear bit before starting (1 stop bit)
    
    if (bits == 1)
    {
        UCSR0C |= (1<<USBS0);                               // 2 stop bits
    }
}

/* Set parity config */
void set_parity(unsigned int bits) {
    UCSR0C &= ~(1<<UPM01) | ~(1<<UPM00);                    // Clear bits before starting (no parity)

    if (bits == 1)
    {
        UCSR0C |= (1<<UPM01);                               // Even parity
    }
    else if (bits == 2)
    {
        UCSR0C |= (1<<UPM01) | (1<<UPM00);                  // Odd parity
    }
}

/* Transmit data over USART */
void usart_transmit(unsigned char data) {
    while(!(UCSR0A & (1<<UDRE0)));                          // Wait for empty transmit buffer
    UDR0 = data;                                            // Put data into buffer and send
}

int usart_receive(void) {
    while(!(UCSR0A & (1<<RXC0)));                           // Wait for data to be recieved
    return UDR0;                                            // Get and return data from buffer
}

/* Initialize the USART communication */
void init_usart(unsigned int baud_rate, unsigned int transmit_rate, unsigned int data_bits, unsigned int stop_bits, unsigned int parity_bits) {
    UBRR0H = (unsigned char) (baud_rate>>8);                // Set baud rate
    UBRR0L = (unsigned char) baud_rate;

    set_transmit_rate(transmit_rate);                       // Data transfer rate

    set_frame_size(data_bits);                              //
    set_stop_bits(stop_bits);                               // Frame specs
    set_parity(parity_bits);                                //

    UCSR0B = (1<<RXEN0) | (1<<TXEN0);                       // Enable Receiver and Transmitter
}