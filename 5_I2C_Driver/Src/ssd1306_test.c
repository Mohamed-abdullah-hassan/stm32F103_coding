
#include "math.h"
#include "ssd1306.h"
#include "stdlib.h"
#include "time.h"
#include "stdint.h"

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
	ssd1306_Clear_Frame();
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
	ssd1306_Clear_Frame();
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
	ssd1306_Clear_Frame();
	for (m = 0; m < 128; m++)
	{
		ssd1306_Draw_Line_H(0, m / 2, m, ssd1306_color_White);
	}
	ssd1306_I2C_Write_Frame();
	//		delay();
	//		delay();
	delay();
	ssd1306_Clear_Frame();
	for (m = 0; m < 128; m++)
	{
		ssd1306_Draw_Line_H(127 - m, m / 2, m, ssd1306_color_White);
	}
	ssd1306_I2C_Write_Frame();

	//		delay();
	//		delay();
	delay();
	ssd1306_Clear_Frame();
	for (m = 0; m < 128; m++)
	{
		ssd1306_Draw_Line_H(0, m / 2, 127 - m, ssd1306_color_White);
	}
	ssd1306_I2C_Write_Frame();
	//				delay();
	delay();
	ssd1306_Clear_Frame();
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
	ssd1306_Clear_Frame();
	ssd1306_Draw_Line_H(0, 0, 28, ssd1306_color_White);
	for (uint8_t h = 1; h < 20; h++)
	{
		x++;
		for (y = 0; y < 9; y++)
		{
			ssd1306_Draw_Line_H(y + offset, x++, h, ssd1306_color_White);
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
	ssd1306_Clear_Frame();
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
	ssd1306_Clear_Frame();
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
	ssd1306_Clear_Frame();
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
	ssd1306_Clear_Frame();
	ssd1306_Draw_Line_V(0, 0, 30, ssd1306_color_White);
	for (uint8_t h = 1; h < 20; h++)
	{
		x++;
		for (y = 1; y < 9; y++)
		{
			ssd1306_Draw_Line_V(x++, y + offset, h, ssd1306_color_White);
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

	ssd1306_Clear_Frame();
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

	for (; x2 >= 20; x2 -= 4)
	{
		ssd1306_Draw_Line(x1, y1, x2, y2, ssd1306_color_White);
		ssd1306_I2C_Write_Frame();
		delay4();
	}
	x2 += 4;
	y2 -= 4;

	for (; y2 >= 10; y2 -= 4)
	{
		ssd1306_Draw_Line(x1, y1, x2, y2, ssd1306_color_White);
		ssd1306_I2C_Write_Frame();
		delay4();
	}
}
void ssd1306_Test_Rect()
{
	ssd1306_Clear_Frame();
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
	delay();
	ssd1306_Clear_Frame();
	volatile double tt = sqrt(121);
	p0.x_point = (uint8_t) tt;
	p0.y_point = (uint8_t) tt;
	ssd1306_Draw_Rectangle(p0, p1, p2, p3, ssd1306_color_White);
	ssd1306_I2C_Write_Frame();
	delay2();
}

void ssd1306_Test_Filled_Rect()
{
	ssd1306_Clear_Frame();
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
