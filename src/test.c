#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	char c;
	char motor;
	int pouring_amount;

	bool isCooling = true;

	while(1) {
		printf("Beginning forever loop");
		scanf(" %1c", &c);

		switch(c) {
			case '0':
				printf("Received a '0' - turn cooler off\n");
				if (isCooling)
					isCooling = false;
				break;
			case '1':
				printf("Received a '1' - turn cooler on\n");
				if (!isCooling)
					isCooling = true;
				break;
			case 'p':
				printf("Received a 'p' - Begin process of pouring a recipe\n");
				while (1) {
					scanf(" %1c", %motor);
					scanf(" %d", %pouring_amount);
					printf("Pouring motor %c for %d ounces\n", motor, pouring_amount);
					break;
				}
				break;
			// default:
		}
	}
	return 0;
}