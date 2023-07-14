#include <stdio.h>
#include "stm32f103xb.h"
#include "LED.h"
#include "uart.h"


void delay(void);

int __io_putchar(int ch)
{
	uart_Send((unsigned char) (ch &0xFF));
	return ch;
}

int main (void)
{
	/*Initialize LED pin*/
	led_Init();

	/*Initialize USART1*/
	uart_TX_Init();

	/*Start Toggling PC13*/

	printf("Hello, From STM32F301C8 MCU\n\r");

	while (1)
	{
		uart_Send('H');
		uart_Send('e');
		uart_Send('l');
		uart_Send('l');
		uart_Send('o');
		uart_Send('!');
		uart_Send('\r');
		led_On();
		delay();
		led_Off();
		delay();


	}
	return 0;
}



void delay(void)
{
	for (volatile int i =0; i<900000; i++);
}
