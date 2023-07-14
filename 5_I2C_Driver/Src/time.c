/*

 * time.c
 *
 *  Created on: Jul 15, 2023
 *      Author: engmo
 */


void delay(void)
{
	for (volatile int i = 0; i < 900000; i++)
		;
}

void delay2(void)
{
	for (volatile int i = 0; i < 90000; i++)
		;
}

void delay3(void)
{
	for (volatile int i = 0; i < 10000; i++)
		;
}

void delay4(void)
{
}
/*
 *
 * SSD1306 Functions test
 *
 */



