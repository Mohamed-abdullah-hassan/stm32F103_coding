
#include "stm32f103xb.h"
#include "LED.h"


void delay(void);

int main (void)
{
	/*Initialize LED pin*/
	led_Init();

	/*Start Toggling PC13*/
	while (1)
	{
		led_On();
		delay();
		led_Off();
		delay();
	}
	return 0;
}


void delay(void)
{
	for (volatile int i =0; i<300000; i++);
}
