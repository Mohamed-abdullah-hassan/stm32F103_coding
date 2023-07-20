#include "stm32f103xb.h"
#include <stdio.h>
#include "stdlib.h"

#include "LED.h"
#include "uart.h"
#include "I2C.h"
#include "ssd1306.h"
#include "ssd1306_test.h"
#include "icon_peugeot.h"




/*
 * End of OLED Display routine
 */

void delay(void);
void delay2(void);

int __io_putchar(int ch)
{
	uart_Send((unsigned char) (ch & 0xFF));
	return ch;
}

int main(void)
{
	/*Initialize LED pin*/
	led_Init();
	/*Initialize USART1*/
	uart_TX_Init();
	/*Initialize I2C1*/
	i2c1_Init();

	/*Make Indication the code is live*/
	/*Start Toggling some LEDs*/
	red_On();
	green_On();
	delay();
	green_Off();
	delay();

	/*Initialize OLED display SSD1306*/
	ssd1306_I2C_Init();

	/*Test the USART and printf function*/
	printf("Hello, From STM32F301C8 MCU\n\r");
	blue_On();
	delay();
	ssd1306_I2C_Set();
	delay();

	ssd1306_I2C_Clear();
	blue_Off();

	while (1)
	{
		uart_Send('H');
		uart_Send('e');
		uart_Send('l');
		uart_Send('l');
		uart_Send('o');
		uart_Send('!');
		uart_Send('\r');
		delay();
		green_Off();
		red_Off();
		blue_Off();
		/* SSD1306 Testing */
		ssd1306_Clear_Frame();
		ssd1306_Draw_Bitmap(0, 0, peugeot, 64, 64);
		ssd1306_I2C_Write_Frame();
		delay();

		ssd1306_Clear_Frame();
//		ssd1306_Test_Fill();
//		ssd1306_Test_Frame_Buffer();
//		ssd1306_Test_Draw_Pixel_2();
//		ssd1306_Test_Draw_Line_H_1();
//		ssd1306_Test_Draw_Line_H_2();
//		ssd1306_Test_Draw_Line_V_1();
//		delay2();
//		ssd1306_Test_Draw_Line_V_1_1();
//		delay2();

		ssd1306_Test_Draw_Line_V_2();
		delay2();
		ssd1306_Test_Draw_Line_H_3();
		delay2();
		ssd1306_Test_Lines();
		delay();
		ssd1306_Test_Rect();
		delay();
		ssd1306_Test_Filled_Rect();
		delay();
//		SSD1306_CLEAR_FRAME;

//		delay();
//		ssd1306_I2C_Clear();
		ssd1306_Fill_buffer(0x5f);
		ssd1306_Draw_Bitmap2(0, 0, peugeot, 64, 64, ssd1306_pixel_XOR);
		ssd1306_Write_Partial_Frame();
		delay();
		delay();
		delay();
		ssd1306_I2C_Clear();
		/* End of SSD1306 Testing*/

		delay();
		delay();
		red_On();
		delay2();
		blue_On();
		green_On();
//		i2c1_End();
		green_Off();
		red_Off();
		blue_Off();
	}
	return 0;
}
