#include <stdio.h>
#include "stm32f103xb.h"
#include "LED.h"
#include "uart.h"
#include "I2C.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"

/*       OLED Display routine
 * SSD1306 commands moving to seperate file after testing
 */
#define SSD1306_I2C_ADDR         0x78
#define SSD1306_RIGHT_HORIZONTAL_SCROLL              0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL               0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL  0x2A
#define SSD1306_DEACTIVATE_SCROLL                    0x2E // Stop scroll
#define SSD1306_ACTIVATE_SCROLL                      0x2F // Start scroll
#define SSD1306_SET_VERTICAL_SCROLL_AREA             0xA3 // Set scroll range

#define SSD1306_NORMALDISPLAY       0xA6
#define SSD1306_INVERTDISPLAY       0xA7

#define SSD1306_Display_Height		64
#define SSD1306_Display_Pages		SSD1306_Display_Height/8
#define SSD1306_Display_Width		128

/*
 * SSD1306 Display Macros
 */
/* Write command */
#define SSD1306_WRITECOMMAND(command)      ssd1306_I2C_Write(SSD1306_I2C_ADDR, 0x00, (command))
/* Write data */
#define SSD1306_WRITEDATA(data)            ssd1306_I2C_Write(SSD1306_I2C_ADDR, 0x40, (data))
/* Clearing Memory Buffer*/
#define SSD1306_CLEAR_FRAME				   memset(Frame_buffer, 0x00, sizeof(Frame_buffer))

/*
 * SSD1306 variables
 */

static uint8_t ssd1306_Frame[SSD1306_Display_Pages * SSD1306_Display_Width];
#define Frame_buffer	ssd1306_Frame
#define check_buffer_size	256

typedef struct
{
	uint8_t buffer0[check_buffer_size];
	uint8_t Frame[SSD1306_Display_Pages * SSD1306_Display_Width];
	uint8_t buffer1[check_buffer_size];
} ssd1306_buffer;

ssd1306_buffer my_Frame;

const uint8_t ssd1306_init[] =
{ (0xAE), //display off
		(0x20), //Set Memory Addressing Mode
		(0x10), //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
		(0xB0), //Set Page Start Address for Page Addressing Mode,0-7
		(0xC8), //Set COM Output Scan Direction
		(0x00), //---set low column address 0x00
		(0x10), //---set high column address 0x10
		(0x40), //--set start line address
		(0x81), //--set contrast control register
		(0x05), // Contrast Value from 0x00 to 0xFF
		(0xA1), //--set segment re-map 0 to 127
		(0xA6), //--set normal display
		(0xA8), //--set multiplex ratio(1 to 64)
		(0x3F), //0x1f for 128*32
		(0xA4), //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
		(0xD3), //-set display offset
		(0x00), //-not offset
		(0xD5), //--set display clock divide ratio/oscillator frequency
		(0xF0), //--set divide ratio
		(0xD9), //--set pre-charge period
		(0x22), //
		(0xDA), //--set com pins hardware configuration
		(0x12), //
		(0xDB), //--set vcomh
		(0x20), //0x20,0.77xVcc
		(0x8D), //--set DC-DC enable
		(0x14), //
		(0xAF), //--turn on SSD1306 panel
		(SSD1306_DEACTIVATE_SCROLL) };

const uint8_t peugeot[] =
{ 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xf0,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x07, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff,
		0xc0, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xc0, 0x7c, 0x00, 0x00,
		0x00, 0xff, 0xff, 0xff, 0xe0, 0xff, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff,
		0xf1, 0xff, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xfb, 0xff, 0x80, 0x00,
		0x00, 0x3c, 0xff, 0xff, 0xff, 0xcf, 0x80, 0x00, 0x00, 0x18, 0x7f, 0xff,
		0xff, 0xc7, 0x80, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0x87, 0xc0, 0x00,
		0x00, 0x0f, 0xff, 0xff, 0xff, 0x87, 0xc0, 0x0f, 0xf0, 0x0f, 0xff, 0xff,
		0xff, 0x87, 0xc0, 0x1f, 0xfc, 0x0f, 0xff, 0xff, 0xff, 0x83, 0xe0, 0x3f,
		0xff, 0x07, 0xbf, 0xff, 0xff, 0xc1, 0xe0, 0x3f, 0xff, 0xc0, 0x3f, 0xff,
		0xff, 0xc0, 0xe0, 0x3f, 0xff, 0xf0, 0x3f, 0xff, 0xff, 0xc0, 0xe0, 0x13,
		0xff, 0xfc, 0x7f, 0xff, 0xff, 0xe0, 0x20, 0x01, 0xff, 0xff, 0xff, 0xff,
		0xff, 0xe0, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00,
		0x3f, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff,
		0xff, 0x78, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xfe, 0x78, 0x00, 0x07,
		0xff, 0xff, 0xff, 0xff, 0xfc, 0x3c, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff,
		0xe0, 0x1c, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0x80, 0x1e, 0x00, 0x7f,
		0xff, 0xff, 0xff, 0xff, 0x80, 0x0e, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff,
		0x80, 0x0f, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x07, 0x00, 0xff,
		0xff, 0xff, 0xff, 0xff, 0xe0, 0x07, 0x00, 0x73, 0xff, 0xff, 0xff, 0xff,
		0xe0, 0x07, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xf0, 0x0f, 0x00, 0x00,
		0x00, 0x03, 0xff, 0xff, 0xf8, 0x1f, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
		0xfc, 0x7f, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
		0x00, 0x00, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff,
		0xff, 0xfc, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00,
		0x00, 0x03, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff,
		0xff, 0xe0, 0x00, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x00,
		0x00, 0x07, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff,
		0xff, 0xf8, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x00,
		0x00, 0x1f, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff,
		0xff, 0xfe, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xe7, 0xff, 0xfe, 0x00, 0x00,
		0x00, 0x3f, 0xff, 0xc3, 0xff, 0xff, 0x00, 0x00, 0x00, 0x7f, 0xff, 0x80,
		0xff, 0xff, 0x00, 0x00, 0x00, 0x7f, 0xff, 0x00, 0x7f, 0xff, 0x80, 0x00,
		0x00, 0x7f, 0xfe, 0x00, 0x3f, 0xff, 0xc0, 0x00, 0x00, 0x7f, 0xfc, 0x00,
		0x0f, 0xff, 0xc0, 0x00, 0x00, 0x7f, 0xf8, 0x00, 0x03, 0xff, 0xe0, 0x00,
		0x00, 0x7f, 0xf0, 0x00, 0x00, 0x7f, 0xe0, 0x00, 0x00, 0xff, 0xf0, 0x00,
		0x00, 0x7f, 0xf0, 0x00, 0x3f, 0xff, 0xe0, 0x00, 0x07, 0xff, 0xf0, 0x00,
		0xff, 0xff, 0xc0, 0x00, 0x1f, 0xff, 0xf8, 0x00, 0xff, 0xff, 0xc0, 0x00,
		0x1f, 0xff, 0xf8, 0x01, 0xff, 0xff, 0xc0, 0x00, 0x3f, 0xff, 0xf8, 0x01,
		0xff, 0xff, 0xc0, 0x00, 0x3f, 0xff, 0xf8, 0x01, 0xff, 0xff, 0xc0, 0x00,
		0x3f, 0xff, 0xf0, 0x01, 0xff, 0xff, 0xc0, 0x00, 0x3f, 0xff, 0xe0, 0x01,
		0xfc, 0xbf, 0xc0, 0x00, 0x3d, 0xbf, 0xc0 };

/*
 * SSD1306 Functions
 */
void ssd1306_I2C_Init()
{

	i2c1_Write_Begin(SSD1306_I2C_ADDR, 0x00);
	for (uint8_t i = 0; i < sizeof(ssd1306_init); i++)
		i2c1_write(ssd1306_init[i]);
	i2c1_End();
//	for (uint16_t i; i < (check_buffer_size); i++)
//	{
//		my_Frame.buffer0[i] = 0xAA;
//		my_Frame.buffer1[i] = 0x55;
//	}

}

void ssd1306_I2C_Write(uint8_t address, uint8_t reg, uint8_t data)
{
	i2c1_write(data);
}
void ssd1306_I2C_Write_Frame()
{
	uint8_t m;
	uint16_t idx = 0;

	for (m = 0; m < 8; m++)
	{
		i2c1_Write_Begin(SSD1306_I2C_ADDR, 0x00);
		i2c1_write(0xB0 + m);
		i2c1_write(0x00);
		i2c1_write(0x10);
		i2c1_End();
		i2c1_Write_Begin(SSD1306_I2C_ADDR, 0x40);
		for (uint8_t i = 0; i < 128; i++)
		{
			idx = (128 * m) + i;
			i2c1_write(ssd1306_Frame[idx]);
		}
		i2c1_End();
	}

}
void ssd1306_I2C_Clear(void)
{
	uint8_t m;

	for (m = 0; m < 8; m++)
	{
		i2c1_Write_Begin(SSD1306_I2C_ADDR, 0x00);
		i2c1_write(0xB0 + m);
		i2c1_write(0x00);
		i2c1_write(0x10);
		i2c1_End();
		i2c1_Write_Begin(SSD1306_I2C_ADDR, 0x40);
		for (uint8_t i = 0; i < 128; i++)
			i2c1_write(0x00);
		i2c1_End();
	}
}

void ssd1306_I2C_Set(void)
{
	uint8_t m;

	for (m = 0; m < 8; m++)
	{
		i2c1_Write_Begin(SSD1306_I2C_ADDR, 0x00);
		i2c1_write(0xB0 + m);
		i2c1_write(0x00);
		i2c1_write(0x10);
		i2c1_End();
		i2c1_Write_Begin(SSD1306_I2C_ADDR, 0x40);
		for (uint8_t i = 0; i < 128; i++)
			i2c1_write(0xFF);
		i2c1_End();
	}
}

void ssd1306_I2C_Fill(uint8_t pattern)
{
	uint8_t m;

	for (m = 0; m < 8; m++)
	{
		i2c1_Write_Begin(SSD1306_I2C_ADDR, 0x00);
		i2c1_write(0xB0 + m);
		i2c1_write(0x00);
		i2c1_write(0x10);
		i2c1_End();
		i2c1_Write_Begin(SSD1306_I2C_ADDR, 0x40);
		for (uint8_t i = 0; i < 128; i++)
			i2c1_write(pattern);
		i2c1_End();
	}
}
void ssd1306_I2C_Fill_char(uint8_t pattern)
{
	uint8_t m;

	for (m = 0; m < 8; m++)
	{
		i2c1_Write_Begin(SSD1306_I2C_ADDR, 0x00);
		i2c1_write(0xB0 + m);
		i2c1_write(0x00);
		i2c1_write(0x10);
		i2c1_End();
		i2c1_Write_Begin(SSD1306_I2C_ADDR, 0x40);
		for (uint8_t i = 0; i < 128 / 2; i++)
		{
			i2c1_write(0x00);
			i2c1_write(pattern);
		}
		i2c1_End();
	}
}
enum ssd1306_color_pallet
{
	ssd1306_color_Black = 0x00, ssd1306_color_White = 0xFF
};
typedef uint8_t ssd1306_color_t;

typedef struct
{
	uint8_t x_point;
	uint8_t y_point;
} ssd1306_point;

void ssd1306_Draw_Pixel(uint8_t x_Pos, uint8_t y_Pos, ssd1306_color_t color)
{
	/* Check of one of the coordinates is out of boundaries*/
	if (x_Pos >= SSD1306_Display_Width)
		return;
	if (y_Pos >= SSD1306_Display_Height)
		return;
	uint8_t temp = 0;
	uint8_t data = 0; /*For more explaining of the code */

	/* Calculate the index of the pixel in frame buffer*/
	/* Get the page number 0 ~ 7*/
	temp = (uint8_t) (y_Pos / 8);

	/* Load the correct address*/
	uint16_t idx = (temp * 128) + x_Pos;
	data = ssd1306_Frame[idx];

	/* Calculate the bit number of the pixel in the byte*/
	uint8_t temp2 = (y_Pos % 8);

	/*Set or clear the pixel according to color*/
	if (color == ssd1306_color_White) /*Set the pixel*/
		data |= (1 << temp2);
	else
		data &= ~(1 << temp2);

	ssd1306_Frame[idx] = data;
}

void ssd1306_Draw_Line_H(uint8_t x_Start, uint8_t y_Start, uint8_t width,
		ssd1306_color_t color)
{
	/* Check if the width is pointing to negative direction and correct it*/
	/*Not Implemented*/

	/* Check if the coordinates is out of display*/
	if ((y_Start > 63) || (x_Start > 127))
		return;

	/*Check if the line runs outside the screen boundaries and crop if true*/
	if ((x_Start + width) > 128)
	{
		width = 127 - x_Start;
	}

	/* Calculate the Page Number in display*/
	uint8_t page = y_Start / 8;

	/* Calculate the Pixel location in the byte*/
	uint8_t bit = y_Start % 8;

	/* Define mask of pixel operation SET or Clear*/
	bit = ((color == ssd1306_color_White) ? (1 << bit) : (~(1 << bit)));

	/* Get the location of the first byte in the frame buffer*/
	uint8_t *data = &ssd1306_Frame[page * 128 + x_Start];

	for (; width > 0; width--)
	{
		if (color == ssd1306_color_White)
			*data |= bit;
		else
			*data &= bit;

		data++;
	}
}

void ssd1306_Draw_Line_V(uint8_t x_Start, uint8_t y_Start, uint8_t height,
		ssd1306_color_t color)
{
	/* Check if the width is pointing to negative direction and correct it*/
	/*Not Implemented*/

	/* Check if the coordinates is out of display*/
	if ((y_Start > 63) || (x_Start > 127))
		return;

	/*Check if the line runs outside the screen boundaries and crop if true*/
	if ((y_Start + height) > 64)
	{
		height = 64 - y_Start;
	}

	/* Get the Page and bit location*/
	uint8_t page = y_Start / 8;
	uint8_t bit = y_Start % 8; /* Indicates the bit location on scale of 0 ~ 7*/

	/* Get the location of the first byte in the frame buffer*/
	uint8_t *data = &ssd1306_Frame[page * 128 + x_Start];

	uint8_t bitMask = 0xFF;

	/* Test if the height of the line is still larger than the starting bit to the end of the byte*/
	while (height > 0)
	{
		if (height > (8 - bit))
		{
			bitMask <<= bit;
			if (color == ssd1306_color_White)
				*data |= bitMask;
			else
				*data &= ~bitMask;
			height -= (8 - bit);
			data += 128;
		}
		else
		{
			bitMask >>= (8 - height);
			bitMask <<= bit;
			if (color == ssd1306_color_White)
				*data |= bitMask;
			else
				*data &= ~bitMask;
			height = 0;

		}
		bit = 0;
		bitMask = 0xFF;
	}
}

void ssd1306_Draw_Line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1,
		ssd1306_color_t color)
{
	/* Check if any of the point is out of the display area and crop it*/
	if (x0 >= SSD1306_Display_Width)
		x0 = SSD1306_Display_Width - 1;
	if (x1 >= SSD1306_Display_Width)
		x1 = SSD1306_Display_Width - 1;
	if (y0 >= SSD1306_Display_Height)
		y0 = SSD1306_Display_Height - 1;
	if (y1 >= SSD1306_Display_Height)
		y1 = SSD1306_Display_Height - 1;

	int16_t dx; //= (x1 - x0);
	int16_t dy; //= -(y1 - y0);
	uint8_t sx = x0 < x1 ? 1 : -1;
	uint8_t sy = y0 < y1 ? 1 : -1;
	int16_t e2; /* error value e_xy */
	dx = (x0 < x1) ? (x1 - x0) : (x0 - x1);
	dy = (y0 < y1) ? (y1 - y0) : (y0 - y1);
	int16_t err = ((dx > dy) ? dx : -dy) / 2;

	if (dx == 0)
	{
		ssd1306_Draw_Line_V(x0, ((y0 < y1) ? (y0) : (y1)), (dy + 1), color);
		return;
	}

	if (dy == 0)
	{
		ssd1306_Draw_Line_H((x0 < x1) ? (x0) : (x1), y0, (dx + 1), color);

		return;
	}

	for (;;)
	{ /* loop */
		ssd1306_Draw_Pixel(x0, y0, color);

		if (x0 == x1 && y0 == y1)
		{
			break;
		}
		e2 = err;
		if (e2 > -dx)
		{
			err -= dy;
			x0 += sx;
		}
		if (e2 < dy)
		{
			err += dx;
			y0 += sy;
		}
	}
}

void ssd1306_Draw_Direct_Rectangle(uint8_t x0, uint8_t y0, uint8_t x1,
		uint8_t y1, ssd1306_color_t color)
{
	ssd1306_Draw_Line(x0, y0, x0, y1, color);
	ssd1306_Draw_Line(x0, y1, x1, y1, color);
	ssd1306_Draw_Line(x1, y0, x1, y1, color);
	ssd1306_Draw_Line(x0, y0, x1, y0, color);

}

void ssd1306_Draw_Recangle_Filled(uint8_t x0, uint8_t y0, uint8_t x1,
		uint8_t y1, ssd1306_color_t color)
{
	if ((x0 > x1) | (y0 > y1))
		return;

	uint8_t dx = x1 - x0;
	uint8_t dy = y1 - y0 + 1;

	for (uint8_t i = 0; i <= dx; i++)
	{
		ssd1306_Draw_Line_V(x0 + i, y0, dy, color);
	}
}

void ssd1306_Draw_Rectangle(ssd1306_point P0, ssd1306_point P1,
		ssd1306_point p2, ssd1306_point P3, ssd1306_color_t color)
{
	ssd1306_Draw_Line(P0.x_point, P0.y_point, P1.x_point, P1.y_point, color);
	ssd1306_Draw_Line(P1.x_point, P1.y_point, p2.x_point, p2.y_point, color);
	ssd1306_Draw_Line(p2.x_point, p2.y_point, P3.x_point, P3.y_point, color);
	ssd1306_Draw_Line(P3.x_point, P3.y_point, P0.x_point, P0.y_point, color);
}

void ssd1306_Draw_Bitmap(uint8_t x_start, uint8_t y_start, const uint8_t *image,
		uint8_t width, uint8_t height)
{
	/*
	 *  Removed as loop check it
	 *	if ((width == 0) | (height == 0))
	 *		return;
	 */
	uint8_t data = 0;
	uint8_t xbit = 0;
	uint8_t w; //= width;
	uint8_t x;
	data = *image++;
	while (height > 0)
	{
		w = width;
		x = x_start;
		while (w > 0)
		{
			ssd1306_Draw_Pixel(x++, y_start,
					((data & (1 << 7)) ?
							ssd1306_color_White : ssd1306_color_Black));
			data <<= 1;
			xbit++;
			if (xbit > 7)
			{
				xbit = 0;
				data = *image++;
			}
			w--;
		}
		y_start++;
		height--;
	}
}

/*SSD1306 Test function Declarations */
void ssd1306_Test_Frame_Buffer();
void ssd1306_Test_Draw_Pixel_2();
void ssd1306_Test_Fill();
void ssd1306_Test_Draw_Line_H_1();
void ssd1306_Test_Draw_Line_H_2();
void ssd1306_Test_Draw_Line_H_3(void);
void ssd1306_Test_Draw_Line_V_1(void);
void ssd1306_Test_Draw_Line_V_2(void);
void ssd1306_Test_Draw_Line_V_1_1(void);
void ssd1306_Test_Lines();
void ssd1306_Test_Rect();
void ssd1306_Test_Filled_Rect();

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

//		SSD1306_CLEAR_FRAME;
		ssd1306_Draw_Bitmap(0, 0, peugeot, 64, 64);
//		ssd1306_Draw_Bitmap2(64, 0, peugeot, 64, 64);
		ssd1306_I2C_Write_Frame();
		delay();
		delay();
		delay();

		SSD1306_CLEAR_FRAME;
//		ssd1306_Test_Fill();
////		ssd1306_Test_Frame_Buffer();
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

		ssd1306_I2C_Clear();
		/* End of SSD1306 Testing*/

		delay();
		delay();
//		delay();
		blue_On();
		green_On();
//		i2c1_End();
		green_Off();
		red_Off();
		blue_Off();
	}
	return 0;
}

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

void ssd1306_Test_Frame_Buffer()
{
	uint8_t m = 0;
	m = 0;
	for (uint8_t i = 0; i < 32; i++)
		ssd1306_Frame[i + m * 128] = 0xFF;
	for (uint8_t i = 32; i < 64; i++)
		ssd1306_Frame[i + m * 128] = 0x0F;
	for (uint8_t i = (64); i < 96; i++)
		ssd1306_Frame[i + m * 128] = 0xFF;
	for (uint8_t i = 96; i < 128; i++)
		ssd1306_Frame[i + m * 128] = 0xF0;
	m++;
	for (uint8_t i = 0; i < 32; i++)
		ssd1306_Frame[i + m * 128] = 0xFF;
	for (uint8_t i = 32; i < 64; i++)
		ssd1306_Frame[i + m * 128] = 0xFF;
	for (uint8_t i = (64); i < 96; i++)
		ssd1306_Frame[i + m * 128] = 0xAA;
	for (uint8_t i = 96; i < 128; i++)
		ssd1306_Frame[i + m * 128] = 0x55;
	m++;
	for (uint8_t i = 0; i < 32; i++)
		ssd1306_Frame[i + m * 128] = 0xFF;
	for (uint8_t i = 32; i < 64; i++)
		ssd1306_Frame[i + m * 128] = 0x5C;
	for (uint8_t i = (64); i < 96; i++)
		ssd1306_Frame[i + m * 128] = 0xC5;
	for (uint8_t i = 96; i < 128; i++)
		ssd1306_Frame[i + m * 128] = 0xFF;
	m++;
	for (uint8_t i = 0; i < 32; i++)
		ssd1306_Frame[i + m * 128] = 0xFF;
	for (uint8_t i = 32; i < 64; i++)
		ssd1306_Frame[i + m * 128] = 0xFF;
	for (uint8_t i = (64); i < 96; i++)
		ssd1306_Frame[i + m * 128] = 0xAA;
	for (uint8_t i = 96; i < 128; i++)
		ssd1306_Frame[i + m * 128] = 0x55;
	m++;
	for (uint8_t i = 0; i < 32; i++)
		ssd1306_Frame[i + m * 128] = 0xFF;
	for (uint8_t i = 32; i < 64; i++)
		ssd1306_Frame[i + m * 128] = 0x0F;
	for (uint8_t i = (64); i < 96; i++)
		ssd1306_Frame[i + m * 128] = 0xFF;
	for (uint8_t i = 96; i < 128; i++)
		ssd1306_Frame[i + m * 128] = 0xF0;
	m++;
	for (uint8_t i = 0; i < 32; i++)
		ssd1306_Frame[i + m * 128] = 0xFF;
	for (uint8_t i = 32; i < 64; i++)
		ssd1306_Frame[i + m * 128] = 0xFF;
	for (uint8_t i = (64); i < 96; i++)
		ssd1306_Frame[i + m * 128] = 0xAA;
	for (uint8_t i = 96; i < 128; i++)
		ssd1306_Frame[i + m * 128] = 0x55;
	m++;
	for (uint8_t i = 0; i < 32; i++)
		ssd1306_Frame[i + m * 128] = 0xFF;
	for (uint8_t i = 32; i < 64; i++)
		ssd1306_Frame[i + m * 128] = 0x5C;
	for (uint8_t i = (64); i < 96; i++)
		ssd1306_Frame[i + m * 128] = 0xC5;
	for (uint8_t i = 96; i < 128; i++)
		ssd1306_Frame[i + m * 128] = 0xFF;
	m++;
	for (uint8_t i = 0; i < 32; i++)
		ssd1306_Frame[i + m * 128] = 0xFF;
	for (uint8_t i = 32; i < 64; i++)
		ssd1306_Frame[i + m * 128] = 0xFF;
	for (uint8_t i = (64); i < 96; i++)
		ssd1306_Frame[i + m * 128] = 0xAA;
	for (uint8_t i = 96; i < 128; i++)
		ssd1306_Frame[i + m * 128] = 0x55;

	ssd1306_I2C_Write_Frame();
	delay();

	m = 0;
	for (uint8_t i = 0; i < 64; i++)
		ssd1306_Frame[i + m * 128] = 0x00;
	for (uint8_t i = (64); i < 128; i++)
		ssd1306_Frame[i + m * 128] = 0xFF;
	m++;
	for (uint8_t i = 0; i < 64; i++)
		ssd1306_Frame[i + m * 128] = 0x00;
	for (uint8_t i = (64); i < 128; i++)
		ssd1306_Frame[i + m * 128] = 0xFF;
	m++;
	for (uint8_t i = 0; i < 64; i++)
		ssd1306_Frame[i + m * 128] = 0x00;
	for (uint8_t i = (64); i < 128; i++)
		ssd1306_Frame[i + m * 128] = 0xFF;
	m++;
	for (uint8_t i = 0; i < 64; i++)
		ssd1306_Frame[i + m * 128] = 0x00;
	for (uint8_t i = (64); i < 128; i++)
		ssd1306_Frame[i + m * 128] = 0xFF;
	m++;
	for (uint8_t i = 0; i < 64; i++)
		ssd1306_Frame[i + m * 128] = 0xFF;
	for (uint8_t i = (64); i < 128; i++)
		ssd1306_Frame[i + m * 128] = 0x0;
	m++;
	for (uint8_t i = 0; i < 64; i++)
		ssd1306_Frame[i + m * 128] = 0xFF;
	for (uint8_t i = (64); i < 128; i++)
		ssd1306_Frame[i + m * 128] = 0x0;
	m++;
	for (uint8_t i = 0; i < 64; i++)
		ssd1306_Frame[i + m * 128] = 0xFF;
	for (uint8_t i = (64); i < 128; i++)
		ssd1306_Frame[i + m * 128] = 0x0;
	m++;
	for (uint8_t i = 0; i < 64; i++)
		ssd1306_Frame[i + m * 128] = 0xFF;
	for (uint8_t i = (64); i < 128; i++)
		ssd1306_Frame[i + m * 128] = 0x0;
	ssd1306_I2C_Write_Frame();
	delay();

	m = 0;
	for (uint8_t i = 0; i < 64; i++)
		ssd1306_Frame[i + m * 128] = 0xFF;
	for (uint8_t i = (64); i < 128; i++)
		ssd1306_Frame[i + m * 128] = 0x0;
	m++;
	for (uint8_t i = 0; i < 64; i++)
		ssd1306_Frame[i + m * 128] = 0xFF;
	for (uint8_t i = (64); i < 128; i++)
		ssd1306_Frame[i + m * 128] = 0x0;
	m++;
	for (uint8_t i = 0; i < 64; i++)
		ssd1306_Frame[i + m * 128] = 0xFF;
	for (uint8_t i = (64); i < 128; i++)
		ssd1306_Frame[i + m * 128] = 0x0;
	m++;
	for (uint8_t i = 0; i < 64; i++)
		ssd1306_Frame[i + m * 128] = 0xFF;
	for (uint8_t i = (64); i < 128; i++)
		ssd1306_Frame[i + m * 128] = 0x0;
	m++;
	for (uint8_t i = 0; i < 64; i++)
		ssd1306_Frame[i + m * 128] = 0x00;
	for (uint8_t i = (64); i < 128; i++)
		ssd1306_Frame[i + m * 128] = 0xFF;
	m++;
	for (uint8_t i = 0; i < 64; i++)
		ssd1306_Frame[i + m * 128] = 0x00;
	for (uint8_t i = (64); i < 128; i++)
		ssd1306_Frame[i + m * 128] = 0xFF;
	m++;
	for (uint8_t i = 0; i < 64; i++)
		ssd1306_Frame[i + m * 128] = 0x00;
	for (uint8_t i = (64); i < 128; i++)
		ssd1306_Frame[i + m * 128] = 0xFF;
	m++;
	for (uint8_t i = 0; i < 64; i++)
		ssd1306_Frame[i + m * 128] = 0x00;
	for (uint8_t i = (64); i < 128; i++)
		ssd1306_Frame[i + m * 128] = 0xFF;

	ssd1306_I2C_Write_Frame();
	delay();
}

void ssd1306_Test_Draw_Pixel_2()
{
	SSD1306_CLEAR_FRAME;
	for (int i = 0; i < 128; i++)
	{
		ssd1306_Draw_Pixel((uint8_t) i, (uint8_t) (i / 2), ssd1306_color_White);
		ssd1306_Draw_Pixel((uint8_t) (127 - i), (uint8_t) (i / 2),
				ssd1306_color_White);
		ssd1306_Draw_Pixel((uint8_t) i, (uint8_t) 32, ssd1306_color_White);
		ssd1306_Draw_Pixel((uint8_t) 64, (uint8_t) (i / 2),
				ssd1306_color_White);
		ssd1306_Draw_Pixel((uint8_t) (i / 4) + 32, (uint8_t) (i / 2),
				ssd1306_color_White);
		ssd1306_Draw_Pixel((uint8_t) (i / 4) + 32, (uint8_t) 32 - (i / 4),
				ssd1306_color_White);
		ssd1306_Draw_Pixel((uint8_t) 64 - (i / 4), (uint8_t) 32 - (i / 4),
				ssd1306_color_White);
	}
	ssd1306_I2C_Write_Frame();
	delay();
}

void ssd1306_Test_Fill()

{
	//		i2c1_Write_Begin(0x78, 0xAE);
	ssd1306_I2C_Fill(0xAA);
	//		delay();
	ssd1306_I2C_Fill(0x55);
	//		delay();
	ssd1306_I2C_Fill(0xF0);
	//		delay();
	ssd1306_I2C_Fill(0x75);
	//		delay();
	ssd1306_I2C_Fill(0x0F);
	//		delay();
	ssd1306_I2C_Fill_char(0x5f);
	//	delay();
	//		delay();
}

void ssd1306_Test_Draw_Line_H_1()
{
//	uint8_t m = 0;
	SSD1306_CLEAR_FRAME;
	ssd1306_Draw_Line_H(0, 0, 128, ssd1306_color_White);
	ssd1306_I2C_Write_Frame();
	//				delay();
	ssd1306_Draw_Line_H(0, 63, 128, ssd1306_color_White);
	ssd1306_I2C_Write_Frame();
	//				delay();
	ssd1306_Draw_Line_H(64, 30, 30, ssd1306_color_White);
	ssd1306_I2C_Write_Frame();
	//				delay();
	ssd1306_Draw_Line_H(32, 15, 128, ssd1306_color_White);
	ssd1306_I2C_Write_Frame();
	//				delay();
	ssd1306_Draw_Line_H(120, 55, 50, ssd1306_color_White);
	ssd1306_I2C_Write_Frame();
	delay();
}

void ssd1306_Test_Draw_Line_H_2()
{
	uint8_t m = 0;
	SSD1306_CLEAR_FRAME;
	for (m = 0; m < 128; m++)
	{
		ssd1306_Draw_Line_H(0, m / 2, m, ssd1306_color_White);
	}
	ssd1306_I2C_Write_Frame();
	//		delay();
	//		delay();
	delay();
	SSD1306_CLEAR_FRAME;
	for (m = 0; m < 128; m++)
	{
		ssd1306_Draw_Line_H(127 - m, m / 2, m, ssd1306_color_White);
	}
	ssd1306_I2C_Write_Frame();

	//		delay();
	//		delay();
	delay();
	SSD1306_CLEAR_FRAME;
	for (m = 0; m < 128; m++)
	{
		ssd1306_Draw_Line_H(0, m / 2, 127 - m, ssd1306_color_White);
	}
	ssd1306_I2C_Write_Frame();
	//				delay();
	delay();
	SSD1306_CLEAR_FRAME;
	for (m = 0; m < 128; m++)
	{
		ssd1306_Draw_Line_H(m, m / 2, 127 - m, ssd1306_color_White);
	}
	ssd1306_I2C_Write_Frame();
	delay();
}

void ssd1306_Test_Draw_Line_H_3(void)
{
	uint8_t x = 1;
	uint8_t y = 1;
	uint8_t offset = 0;
	SSD1306_CLEAR_FRAME;
	ssd1306_Draw_Line_H(0, 0, 28, ssd1306_color_White);

	for (uint8_t h = 1; h < 20; h++)
	{
		x++;
		for (y = 0; y < 9; y++)
		{
			ssd1306_Draw_Line_H(y + offset, x++, h, ssd1306_color_White);
//				delay2();
			if (x >= (64 - 7))
			{
				x = 0;
				offset += 31;
				ssd1306_Draw_Line_V(offset - 2, 0, 64, ssd1306_color_White);
			}
		}
		x++;

		ssd1306_Draw_Line_H(offset, x, 28, ssd1306_color_White);
		x++;

	}
	ssd1306_I2C_Write_Frame();
}

void ssd1306_Test_Draw_Line_V_1(void)
{
	SSD1306_CLEAR_FRAME;
//	memset(ssd1306_Frame, 0xFF, sizeof(ssd1306_Frame));
	ssd1306_Draw_Line_V(20, 5, 30, ssd1306_color_White);
	//		ssd1306_Draw_Line_V(40, 0, 63, ssd1306_color_White);
	//		ssd1306_Draw_Line_V(127, 8, 50, ssd1306_color_White);
	ssd1306_Draw_Line_V(0, 0, 63, ssd1306_color_White);
	ssd1306_Draw_Line_V(127, 0, 63, ssd1306_color_White);

	ssd1306_I2C_Write_Frame();
//	delay();
//	delay();
	delay();
	SSD1306_CLEAR_FRAME;
	ssd1306_Draw_Line_H(32, 16, 64, ssd1306_color_White);
	ssd1306_Draw_Line_V(32, 16, 32, ssd1306_color_White);
	ssd1306_Draw_Line_H(32, 16 + 32, 64, ssd1306_color_White);
	ssd1306_Draw_Line_V(32 + 64, 16, 32, ssd1306_color_White);

	ssd1306_I2C_Write_Frame();
	delay();
}
void ssd1306_Test_Draw_Line_V_1_1(void)
{
//	SSD1306_CLEAR_FRAME;
	memset(ssd1306_Frame, 0xFF, sizeof(ssd1306_Frame));
//	ssd1306_Draw_Line_V(20, 5, 30, ssd1306_color_Black);
//	//		ssd1306_Draw_Line_V(40, 0, 63, ssd1306_color_White);
//	//		ssd1306_Draw_Line_V(127, 8, 50, ssd1306_color_White);
//	ssd1306_Draw_Line_V(0, 0, 63, ssd1306_color_Black);
//	ssd1306_Draw_Line_V(127, 0, 63, ssd1306_color_Black);

	ssd1306_I2C_Write_Frame();
//	delay();
//	delay();
//	delay();
//	SSD1306_CLEAR_FRAME;
	ssd1306_Draw_Line_H(32, 16, 64, ssd1306_color_Black);
	ssd1306_Draw_Line_V(32, 16, 32, ssd1306_color_Black);
	ssd1306_Draw_Line_H(32, 16 + 32, 64, ssd1306_color_Black);
	ssd1306_Draw_Line_V(32 + 64, 16, 32, ssd1306_color_Black);

	ssd1306_I2C_Write_Frame();
	delay();
}

void ssd1306_Test_Draw_Line_V_2(void)
{
	uint8_t x = 1;
	uint8_t y = 1;
	uint8_t offset = 0;
	SSD1306_CLEAR_FRAME;
	ssd1306_Draw_Line_V(0, 0, 30, ssd1306_color_White);
	for (uint8_t h = 1; h < 20; h++)
	{
		x++;
		for (y = 1; y < 9; y++)
		{
			ssd1306_Draw_Line_V(x++, y + offset, h, ssd1306_color_White);
			//				ssd1306_I2C_Write_Frame();
			//				delay2();
			if (x >= (127 - 7))
			{
				x = 0;
				offset += 32;
			}
		}

		ssd1306_Draw_Line_V(++x, offset, 30, ssd1306_color_White);
		x++;

	}
	ssd1306_I2C_Write_Frame();
}
void ssd1306_Test_Lines_2()
{
	//	SSD1306_CLEAR_FRAME;
	//	ssd1306_I2C_Write_Frame();
	//
	//	x1 = y1 = 0;
	//	y2 = SSD1306_Display_Height - 1;
	//	for (x2 = 0; x2 < SSD1306_Display_Width; x2 += 6)
	//	{
	//		ssd1306_Draw_Line(x1, y1, x2, y2, ssd1306_color_White);
	//		ssd1306_I2C_Write_Frame();
	//		delay4();
	//	}
	//	x2 = SSD1306_Display_Width - 1;
	//	for (y2 = 0; y2 < SSD1306_Display_Height; y2 += 6)
	//	{
	//		ssd1306_Draw_Line(x1, y1, x2, y2, ssd1306_color_White);
	//		ssd1306_I2C_Write_Frame();
	//		delay4();
	//	}
	//
	//	delay();
	//
	//	SSD1306_CLEAR_FRAME;
	//	ssd1306_I2C_Write_Frame();
	//
	//	x1 = SSD1306_Display_Width - 1;
	//	y1 = 0;
	//	y2 = SSD1306_Display_Height - 1;
	//	for (x2 = 0; x2 < SSD1306_Display_Width; x2 += 6)
	//	{
	//		ssd1306_Draw_Line(x1, y1, x2, y2, ssd1306_color_White);
	//		ssd1306_I2C_Write_Frame();
	//		delay4();
	//	}
	//	x2 = 0;
	//	for (y2 = 0; y2 < SSD1306_Display_Height; y2 += 6)
	//	{
	//		ssd1306_Draw_Line(x1, y1, x2, y2, ssd1306_color_White);
	//		ssd1306_I2C_Write_Frame();
	//		delay4();
	//	}
	//
	//	delay();
	//
	//	SSD1306_CLEAR_FRAME;
	//	ssd1306_I2C_Write_Frame();
	//	x1 = 0;
	//	y1 = SSD1306_Display_Height - 1;
	//	y2 = 0;
	//	for (x2 = 0; x2 < SSD1306_Display_Width; x2 += 6)
	//	{
	//		ssd1306_Draw_Line(x1, y1, x2, y2, ssd1306_color_White);
	//		ssd1306_I2C_Write_Frame();
	//		delay4();
	//	}
	//	x2 = SSD1306_Display_Width - 1;
	//	for (y2 = 0; y2 < SSD1306_Display_Height; y2 += 6)
	//	{
	//		ssd1306_Draw_Line(x1, y1, x2, y2, ssd1306_color_White);
	//		ssd1306_I2C_Write_Frame();
	//		delay4();
	//	}
	//
	//	delay();
	//
	//	SSD1306_CLEAR_FRAME;
	//	ssd1306_I2C_Write_Frame();
	//
	//	x1 = SSD1306_Display_Width - 1;
	//	y1 = SSD1306_Display_Height - 1;
	//	y2 = 0;
	//	for (x2 = 0; x2 < SSD1306_Display_Width; x2 += 6)
	//	{
	//		ssd1306_Draw_Line(x1, y1, x2, y2, ssd1306_color_White);
	//		ssd1306_I2C_Write_Frame();
	//		delay4();
	//	}
	//	x2 = 0;
	//	for (y2 = 0; y2 < SSD1306_Display_Height; y2 += 6)
	//	{
	//		ssd1306_Draw_Line(x1, y1, x2, y2, ssd1306_color_White);
	//		ssd1306_I2C_Write_Frame();
	//		delay4();
	//	}

}
void ssd1306_Test_Lines()
{
	uint8_t x1, y1, x2, y2;

	SSD1306_CLEAR_FRAME;
	ssd1306_I2C_Write_Frame();
	ssd1306_Draw_Direct_Rectangle(18, 8, 110, 56, ssd1306_color_White);
	ssd1306_I2C_Write_Frame();

	x1 = SSD1306_Display_Width / 2;
	y1 = SSD1306_Display_Height / 2;
	y2 = 10;
	for (x2 = 20; x2 <= (SSD1306_Display_Width - 20); x2 += 4)
	{
		ssd1306_Draw_Line(x1, y1, x2, y2, ssd1306_color_White);
		ssd1306_I2C_Write_Frame();
		delay4();
	}
	x2 -= 4;
	y2 += 4;
	for (y2 = 10; y2 <= (SSD1306_Display_Height - 10); y2 += 4)
	{
		ssd1306_Draw_Line(x1, y1, x2, y2, ssd1306_color_White);
		ssd1306_I2C_Write_Frame();
		delay4();
	}
	y2 -= 4;
	x2 -= 4;
//	x2 = (SSD1306_Display_Width - 20)
	for (; x2 >= 20; x2 -= 4)
	{
		ssd1306_Draw_Line(x1, y1, x2, y2, ssd1306_color_White);
		ssd1306_I2C_Write_Frame();
		delay4();
	}
	x2 += 4;
	y2 -= 4;
//	y2 = (SSD1306_Display_Height - 10)
	for (; y2 >= 10; y2 -= 4)
	{
		ssd1306_Draw_Line(x1, y1, x2, y2, ssd1306_color_White);
		ssd1306_I2C_Write_Frame();
		delay4();
	}

}
void ssd1306_Test_Rect()
{
	SSD1306_CLEAR_FRAME;
	ssd1306_I2C_Write_Frame();
	ssd1306_Draw_Direct_Rectangle(16, 8, 80, 40, ssd1306_color_White);
	ssd1306_I2C_Write_Frame();
	delay2();
	ssd1306_Draw_Direct_Rectangle(0, 0, 63, 31, ssd1306_color_White);
	ssd1306_I2C_Write_Frame();
	delay2();
	ssd1306_Draw_Direct_Rectangle(63, 31, 130, 80, ssd1306_color_White);
	ssd1306_I2C_Write_Frame();
	delay2();
	ssd1306_Draw_Direct_Rectangle(5, 60, 250, 255, ssd1306_color_White);
	ssd1306_I2C_Write_Frame();
	delay2();
	ssd1306_point p0, p1, p2, p3;
	p0.x_point = 63;
	p0.y_point = 10;
	p1.x_point = 20;
	p1.y_point = 31;
	p2.x_point = 63;
	p2.y_point = 53;
	p3.x_point = 108;
	p3.y_point = 31;

	ssd1306_Draw_Rectangle(p0, p1, p2, p3, ssd1306_color_White);
	ssd1306_I2C_Write_Frame();

	volatile double tt = sqrt(121);

	p0.x_point = (uint8_t) tt;
	p0.y_point = (uint8_t) tt;
	ssd1306_Draw_Rectangle(p0, p1, p2, p3, ssd1306_color_White);

}

void ssd1306_Test_Filled_Rect()
{
	SSD1306_CLEAR_FRAME;
	ssd1306_I2C_Write_Frame();
	ssd1306_Draw_Recangle_Filled(20, 10, 64, 55, ssd1306_color_White);
	ssd1306_I2C_Write_Frame();
	delay();
	ssd1306_Draw_Recangle_Filled(66, 5, 255, 120, ssd1306_color_White);
	ssd1306_I2C_Write_Frame();
	delay();
	ssd1306_Draw_Recangle_Filled(127, 64, 0, 0, ssd1306_color_White);
	ssd1306_I2C_Write_Frame();
	delay();
}
