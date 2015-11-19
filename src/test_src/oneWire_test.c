#define F_CPU 16000000UL

/* Standard headers */
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>

#include "../lib/usart/usart.c"
#include "../lib/serial/serial.c"

#define BAUD 9600									// Set baud for serial communication
#define BAUDRATE ((F_CPU) / (BAUD * 16UL) - 1)		// ---------------------------------

#define TRANSMIT_RATE 1								// Set usart info
#define DATA_BITS 8									// --------------
#define STOP_BITS 1									//
#define PARITY_BITS 0								//

#define THERM_PORT		PORTB
#define THERM_DDR		DDRB
#define THERM_PIN		PINB
#define THERM_IO		0

#define ClearBit(x,y) 	x &= ~_BV(y)
#define SetBit(x,y) 	x |= _BV(y)
#define ReadBit(x,y) 	x & _BV(y)

#define THERM_INPUT() 	ClearBit(THERM_DDR,THERM_IO)
#define THERM_OUTPUT() 	SetBit(THERM_DDR,THERM_IO)
#define THERM_LOW() 	ClearBit(THERM_PORT,THERM_IO)
#define THERM_HIGH() 	SetBit(THERM_PORT,THERM_IO)
#define THERM_READ()	ReadBit(THERM_PIN,THERM_IO)

#define THERM_CONVERTTEMP	0x44
#define THERM_READSCRATCH	0xBE
#define THERM_WRITESCRATCH	0x4E
#define THERM_COPYSCRATCH	0x48
#define THERM_READPOWER		0xB4
#define THERM_SEARCHROM		0xF0
#define THERM_READROM		0x33
#define THERM_MATCHROM		0x55
#define THERM_SKIPROM		0xCC
#define THERM_ALARMSEARCH	0xEC

typedef uint8_t byte;
typedef int8_t sbyte;

byte rom[] = {};

void InitAvr() {
	DDRB = 0x00;
}

void msDelay(int delay) {
	int i;
	for (i = 0; i < delay; i++) {
		_delay_ms(1);
	}
}

// pull data line low for 480 seconds to begin transfer
// wait 60 us, then DS18B20 will pull line low to indicate presence

/* Reset operation */
byte therm_Reset(void) {
	byte i;
	THERM_OUTPUT();
	THERM_LOW();
	_delay_us(480);
	THERM_INPUT();
	_delay_us(60);
	i = THERM_READ();
	_delay_us(420);
	return i;
}

// Writing a bit to a slave device: timing slot=60us, beginning with pulling down for 1 us
// To write a 1: release line, letting it float back t logic high state
// To write a 0: hold line low for entier 60us time slot

void therm_WriteBit(byte bit) {
	THERM_OUTPUT();
	THERM_LOW();
	_delay_us(1);
	if (bit) {
		THERM_INPUT();
	}
	_delay_us(60);
	THERM_INPUT();
}

// Reading bits is also 60us time slot. Hold down for 1us, then wait 14us before reading

/* reading and writing bytes is just 8 consecutiv bit operations */
byte therm_ReadBit(void) {
	byte bit = 0;
	THERM_OUTPUT();
	THERM_LOW();
	_delay_us(1);
	THERM_INPUT();
	_delay_us(14);
	if (THERM_READ()) {
		bit = 1;
	}
	_delay_us(45);
	return bit;
}

void therm_WriteByte(byte data) {
	byte i = 8;
	while (i--) {
		therm_WriteBit(data&1);		// send lsb
		data >>= 1;					// shift all bits right
	}
}

byte therm_ReadByte() {
	byte i = 8;
	byte data = 0;
	while (i--) {
		data >>= 1;						// shift all bits right
		data |= (therm_ReadBit()<<7);	// get next bit, lsb first
	}
	return data;
}

void therm_MatchRom(byte rom[]) {
	byte i;
	therm_WriteByte(THERM_MATCHROM);
	for (i = 0; i < 8; i++) {
		therm_WriteByte(rom[i]);
	}
}

void therm_ReadTempRaw(byte id[], byte *t0, byte *t1) {
	therm_Reset();
	if (id) {
		therm_MatchRom(id);
	}
	else {
		therm_WriteByte(THERM_SKIPROM);
	}
	therm_WriteByte(THERM_CONVERTTEMP);	// do temp conversion
	while (!therm_ReadBit());			// wait for it to finish

	therm_Reset();
	if (id) {
		therm_MatchRom(id);
	}
	else {
		therm_WriteByte(THERM_SKIPROM);
	}
	therm_WriteByte(THERM_READSCRATCH);
	*t0 = therm_ReadByte();
	*t1 = therm_ReadByte();
}

void therm_ReadTempC(byte id[], int *whole, int *decimal) {
	byte t0;
	byte t1;
	therm_ReadTempRaw(id, &t0, &t1);
	*whole = (t1 & 0x07) << 4;
	*whole |= t0 >> 4;
	*decimal = t0 & 0x0F;
	*decimal *= 625;
}

void therm_ReadTempF(byte id[], int *whole, int *decimal) {
	byte t0;
	byte t1;
	therm_ReadTempRaw(id, &t0, &t1);
	int t16 = (t1 << 8) + t0;
	int t2 = t16 / 8;
	int f10 = t16 + t2 + 320;
	*whole = f10 / 10;
	*decimal = f10 % 10;
}

void RomReaderProgram() {
	printf("ID (ROM) Reader: ");
	while (1) {
		byte i;
		therm_Reset();
		therm_WriteByte(THERM_READROM);
		for (i = 0; i < 8; i++) {
			byte data = therm_ReadByte();
			printf("%X", data);
		}
		msDelay(1000);
	}
}

// void ReadTemp() {
// 	while (1) {
// 		int whole = 0;
// 		int decimal = 0;

// 		therm_ReadTempF(rom, &whole, &decimal);
// 		printf("Temp(F): %d.%d", whole, decimal);
// 	}
// }

int main(void) {
	init_usart(BAUDRATE, TRANSMIT_RATE, DATA_BITS, STOP_BITS, PARITY_BITS);			// Initialize usart
	stdin = stdout = &usart0_str;			// Set standard streams to serial
	InitAvr();
	RomReaderProgram();
	return 1;
}