#define F_CPU 16000000UL

// Standard headers
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "../Library/lcd/lcd.h"
#include "../Library/lcd/myutils.h"
#include "../Library/lcd/lcd.c"

ISR(PCINT2_vect)
{
	unsigned char i;
	LCDClear();
	LCDWriteString("NICE JOB!");
	for(i=0;i<25;i++) _delay_loop_2(0);
	LCDClear();
	LCDWriteString("Try another!");
}

int main(int argc, const char *argv[])
{
	// unsigned char i;

	LCDInit(LS_NONE);
	LCDClear();


	LCDWriteString("Please press OK");

	// Set as inputs
	DDRC = 0x00;
	// Set pull-up resistors
	PORTC = 0xFF;

	/*	Pin Configuration for switch
		   - PC0 = center
		   - PC1 = left
		   - PC2 = up
		   - PC3 = right
		   - PC4 = down				*/

	// Pin Change interrupt on PC0
	PCICR |= (1<<PCIE2);
	PCMSK2 |= (1<<PCINT20) | (1<<PCINT19) | (1<<PCINT18) | (1<<PCINT17) | (1<<PCINT16);

	sei();

	while(1)
	{

	}


//	while((PINC & (1<<PC0)));



	// while(1)
	// {
	// 	//Clear the screen
	// 	LCDClear();


	// 	//Simple string printing
	// 	LCDWriteString("    Mr. Pour");

	// 	//A string on line 2
	// 	LCDWriteStringXY(0,1," Senior Project");

	// 	//Wait
	// 	for(i=0;i<100;i++) _delay_loop_2(0);

	// 	//Clear the screen
	// 	LCDClear();


	// 	//Simple string printing
	// 	LCDWriteStringXY(0,1,"Loading:");

	// 	//Print some numbers
	// 	for (i=0;i<101;i+=1)
	// 	{
	// 		LCDWriteIntXY(9,1,i,3);
	// 		LCDWriteStringXY(12,1,"%");
	// 		_delay_loop_2(0);
	// 		_delay_loop_2(0);
	// 		_delay_loop_2(0);
	// 		_delay_loop_2(0);
	// 	}

	// 	//Wait
	// 	for(i=0;i<100;i++) _delay_loop_2(0);
	// }

	return 0;
}