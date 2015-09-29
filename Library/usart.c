#include "usart.h"
#include <avr/io.h>

void set_transmit_rate(unsigned int transmit)
{
    // Clear bit
    UCSR0A &= ~(1<<U2X0);

    // Set transmission rate
    if (transmit == 1)
    {
        // Double transmission rate (good for running at 16Mhz clock speed)
        UCSR0A = (1<<U2X0);
    }
}

void set_frame_size(unsigned int bits)
{
    // Clear bits
    UCSR0C &= ~(1<<UCSZ02) | ~(1<<UCSZ01) | ~(1<<UCSZ00);

    if (bits == 6)
    {
        UCSR0C |= (1<<UCSZ00);
    }
    else if (bits == 7)
    {
        UCSR0C |= (1<<UCSZ01);
    }
    else if (bits == 8)
    {
        UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);
    }
    else if (bits == 9)
    {
        UCSR0C |= (1<<UCSZ02) | (1<<UCSZ01) | (1<<UCSZ00);
    }
}

void set_stop_bits(unsigned int bits)
{
    // Clear bit
    UCSR0C &= ~(1<<USBS0);
    
    if (bits == 1)
    {
        UCSR0C |= (1<<USBS0);   
    }
}

void set_parity(unsigned int bits)
{
    // Clear bits
    UCSR0C &= ~(1<<UPM01) | ~(1<<UPM00);

    if (bits == 1)
    {
        // Even parity
        UCSR0C |= (1<<UPM01);
    }
    else if (bits == 2)
    {
        // Odd parity
        UCSR0C |= (1<<UPM01) | (1<<UPM00);
    }
}

void usart_transmit(unsigned char data)
{
    // Wait for empty transmit buffer
    while(!(UCSR0A & (1<<UDRE0)));

    // Put data into buffer and send
    UDR0 = data;
}

unsigned char usart_receive(void)
{
    // Wait for data to be recieved
    while(!(UCSR0A & (1<<RXC0)));

    // Get and return data from buffer
    return UDR0;
}

void init_usart(unsigned int baud_rate, unsigned int transmit_rate, unsigned int data_bits, unsigned int stop_bits, unsigned int parity_bits)
{
    // Set baud rate
    UBRR0H = (unsigned char) (baud_rate>>8);
    UBRR0L = (unsigned char) baud_rate;

    // Data transfer rate
    set_transmit_rate(transmit_rate);

    // Frame specs
    set_frame_size(data_bits);
    set_stop_bits(stop_bits);
    set_parity(parity_bits);

    // Enable Receiver and Transmitter
    UCSR0B = (1<<RXEN0) | (1<<TXEN0);
}