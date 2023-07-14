#include <stdio.h>
#include "stm32f103xb.h"
#include "LED.h"
#include "uart.h"
#include "timebase.h"

//void delay(void);

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

	timebase_init();

	/*Start Toggling PC13*/
	printf("Hello, From STM32F301C8 MCU\n\r");

	while (1)
	{
//		uart_Send('H');
//		uart_Send('e');
//		uart_Send('l');
//		uart_Send('l');
//		uart_Send('o');
//		uart_Send('!');
//		uart_Send('\r');
		led_On();
//		delay();
		delay(1);
		printf("One Second!!\n\r");
		led_Off();
		delay(1);
		printf("One Second- total time %ld\n\r",get_tick());
//		delay();


	}
	return 0;
}
