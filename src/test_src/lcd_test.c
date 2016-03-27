#define F_CPU 16000000UL

// Standard headers
#include <avr/io.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include "../lib/lcd/lcd.h"
#include "../lib/lcd/myutils.h"
#include "../lib/lcd/lcd.c"


int main(int argc, const char *argv[])
{
	LCDInit(LS_NONE);
	LCDClear();

	// while(1)
	// {
		//Clear the screen
		// LCDClear();


		//Simple string printing
		LCDWriteString("GET A STRING!");

		//A string on line 2
		// LCDWriteStringXY(0,1," Senior Project");
		// int i;
		//Wait
		// for(i=0;i<100;i++) _delay_loop_2(0);

		//Clear the screen
		// LCDClear();
	// }

	return 0;
}