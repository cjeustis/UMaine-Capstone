#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	printf("\e[1;1H\e[2J");

	printf("Beginning of terminal\n");

	return 0;
}