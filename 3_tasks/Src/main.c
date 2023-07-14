#include <stdio.h>
#include "stm32f103xb.h"
#include "LED.h"
#include "uart.h"
#include "timebase.h"

//void delay(void);
#include <stdint.h>
//volatile uint32_t myVar[200] __attribute__ ((section (".bss"))) ; //Create an array in .bss section and init it to zeros.
//volatile uint32_t sizo = sizeof(myVar);

void task1 (void);
void task2 (void);
void task3 (void);
void task4 (void);

int __io_putchar(int ch)
{
	uart_Send((unsigned char) (ch &0xFF));
	return ch;
}

int main (void)
{
	/*Initialize LED pin*/
	led_Init();

//	for (int i =0 ; i < sizeof(myVar);i++)
//		myVar[i]=1;
	/*Initialize USART1*/
	uart_TX_Init();

	timebase_init();



	/*Start Toggling PC13*/
	printf("Hello, From STM32F301C8 MCU\n\r");

	while (1)
	{
		task1();
		delay(350);
		task2();
		delay(640);
		task3();
		delay(10);
//		delay();
//		printf("One Second!!\n\r");
		task4();



	}
	return 0;
}

void task1 (void)
{
	red_On();
	green_Off();
	blue_Off();
}
void task2 (void)
{
	red_Off();
	green_On();
	blue_Off();
}
void task3 (void)
{
	red_Off();
	green_Off();
	blue_On();
}

void task4 (void)
{
	printf("One Second- total time %ld\n\r",get_tick());
}
