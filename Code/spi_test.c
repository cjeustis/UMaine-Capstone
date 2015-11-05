#define F_CPU 16000000UL

// Standard headers
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>


void SPI_SlaveInit(void) {
	/* Set MISO output, all others input */
	DDR_SPI = (1<<DD_MISO);
	/* Enable SPI */
	SPCR = (1<<SPE);
}

char SPI_SlaveReceive(void) {
	/* Wait for reception complete */
	while(!(SPSR & (1<<SPIF)));
	/* Return Data Register */
	return SPDR;
}

void SPI_MasterTransmit(char data) {
	// Start transmission
	SPDR = data;
	// Wait for tranmission to complete
	while(!(SPSR & (1<<SPIF)));
}

void SPI_MasterInit(void) {
	// Set MOSI and SCK output, all others input
	DDR_SPI = (1<<DD_MOSI) | (1<<DD_SCK);
	// Enable SPI, set clock rate fck/16
	SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0);
}

int main(int argc, const char *argv[])
{
	init_spi();

	spi_transmit();

	return 0;
}