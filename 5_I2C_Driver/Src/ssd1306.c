#include "I2C.h"
//#include "string.h"
#include "ssd1306.h"

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

/*
 * SSD1306 Display Macros
 */
/* Write command */
#define SSD1306_WRITECOMMAND(command)      ssd1306_I2C_Write(SSD1306_I2C_ADDR, 0x00, (command))
/* Write data */
#define SSD1306_WRITEDATA(data)            ssd1306_I2C_Write(SSD1306_I2C_ADDR, 0x40, (data))

/*
 * SSD1306 variables
 */

static uint8_t ssd1306_Frame[SSD1306_Display_Pages * SSD1306_Display_Width];

struct __display_Buffer
{
	uint8_t y_Page_Start;
	uint8_t y_Page_End;
	uint8_t x_Col_Start;
	uint8_t x_Col_End;
	uint8_t Frame[SSD1306_Display_Pages * SSD1306_Display_Width];
} display_Buffer;

const uint8_t ssd1306_init[] =
{ (0xAE),     //display off
  (0x20),     //Set Memory Addressing Mode
  (0x10),     //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
  (0xB0),     //Set Page Start Address for Page Addressing Mode,0-7
  (0xC8),     //Set COM Output Scan Direction
  (0x00),     //---set low column address 0x00
  (0x10),     //---set high column address 0x10
  (0x40),     //--set start line address
  (0x81),     //--set contrast control register
  (0x05),     // Contrast Value from 0x00 to 0xFF
  (0xA1),     //--set segment re-map 0 to 127
  (0xA6),     //--set normal display
  (0xA8),     //--set multiplex ratio(1 to 64)
  (0x3F),     //0x1f for 128*32
  (0xA4),     //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
  (0xD3),     //-set display offset
  (0x00),     //-not offset
  (0xD5),     //--set display clock divide ratio/oscillator frequency
  (0xF0),     //--set divide ratio
  (0xD9),     //--set pre-charge period
  (0x22),     //
  (0xDA),     //--set com pins hardware configuration
  (0x12),     //
  (0xDB),     //--set vcomh
  (0x20),     //0x20,0.77xVcc
  (0x8D),     //--set DC-DC enable
  (0x14),     //
  (0xAF),     //--turn on SSD1306 panel
  (SSD1306_DEACTIVATE_SCROLL) };

/*
 * SSD1306 Functions
 */
void ssd1306_I2C_Init( )
{

	i2c1_Write_Begin(SSD1306_I2C_ADDR, 0x00);
	for (uint8_t i = 0; i < sizeof(ssd1306_init); i++)
		i2c1_write(ssd1306_init[i]);
	i2c1_End();

	display_Buffer.x_Col_End = 0x00;
	display_Buffer.y_Page_End = 0x00;
	display_Buffer.x_Col_Start = SSD1306_Display_Height - 1;
	display_Buffer.y_Page_Start = SSD1306_Display_Width - 1;

}

void ssd1306_I2C_Write(uint8_t address , uint8_t reg , uint8_t data )
{
	i2c1_write(data);
}

void ssd1306_Pixel_Set(uint8_t x_Pos , uint8_t y_Pos , ssd1306_pixel_op operation ,
        ssd1306_color_t color )
{
	/* Check of one of the coordinates is out of boundaries*/
	if (x_Pos >= SSD1306_Display_Width) return;
	if (y_Pos >= SSD1306_Display_Height) return;

	/* Calculate the index of the pixel in frame buffer*/
	/* Get the page number 0 ~ 7*/
	uint8_t page = (uint8_t) (y_Pos / 8);

	/* Calculate the bit number of the pixel in the byte*/
	uint8_t temp = (y_Pos % 8);

	/*Set or clear the pixel according to color*/
	temp = 1 << temp;
	if (operation == ssd1306_pixel_Set)
	{
		if (color == ssd1306_color_White) /*Set the pixel*/
			display_Buffer.Frame[(page * 128) + x_Pos] |= (temp);
		else display_Buffer.Frame[(page * 128) + x_Pos] &= ~(temp);
	}
	else if (operation == ssd1306_pixel_OR)
	{
		if (color == ssd1306_color_White) display_Buffer.Frame[(page * 128) + x_Pos] |= (temp);
	}
	else if (operation == ssd1306_pixel_AND)
	{
		if (color == ssd1306_color_Black) display_Buffer.Frame[(page * 128) + x_Pos] &= ~(temp);
	}
	else if (operation == ssd1306_pixel_XOR)
	{

		uint8_t new = display_Buffer.Frame[(page * 128) + x_Pos] & temp;
		if (color == ssd1306_color_White)
			new ^= temp;
		else new ^= 0x00;
		display_Buffer.Frame[(page * 128) + x_Pos] =
		        ((display_Buffer.Frame[(page * 128) + x_Pos] & (~temp)) | new);
	}

	if (display_Buffer.x_Col_Start > x_Pos) display_Buffer.x_Col_Start = x_Pos;
	if (display_Buffer.y_Page_Start > page) display_Buffer.y_Page_Start = page;
	if (display_Buffer.x_Col_End < x_Pos) display_Buffer.x_Col_End = x_Pos;
	if (display_Buffer.y_Page_End < page) display_Buffer.y_Page_End = page;
}

void ssd1306_I2C_Write_Frame( )
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

void ssd1306_Write_Partial_Frame( )
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
			i2c1_write(display_Buffer.Frame[idx]);
		}
		i2c1_End();
	}
}

void ssd1306_Clear_Frame( )
{
	for (uint16_t i = 0; i < sizeof(Frame_buffer); i++)
	{
		Frame_buffer[i] = 0x00;
		display_Buffer.Frame[i] = 0x00;
	}
	display_Buffer.x_Col_End = 0x00;
	display_Buffer.y_Page_End = 0x00;
	display_Buffer.x_Col_Start = SSD1306_Display_Height - 1;
	display_Buffer.y_Page_Start = SSD1306_Display_Width - 1;

//	memset(Frame_buffer, 0x00, sizeof(Frame_buffer));
}
void ssd1306_I2C_Clear(void )
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

void ssd1306_I2C_Set(void )
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

void ssd1306_I2C_Fill(uint8_t pattern )
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
void ssd1306_I2C_Fill_char(uint8_t pattern )
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
void ssd1306_Fill_buffer(uint8_t pattern )
{
	for (uint16_t i = 0; i < sizeof(display_Buffer.Frame); i += 2)
	{
		display_Buffer.Frame[i] = 0x00;
		display_Buffer.Frame[i + 1] = pattern;

	}
}

void ssd1306_Draw_Pixel(uint8_t x_Pos , uint8_t y_Pos , ssd1306_color_t color )
{
	/* Check of one of the coordinates is out of boundaries*/
	if (x_Pos >= SSD1306_Display_Width) return;
	if (y_Pos >= SSD1306_Display_Height) return;
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
	else data &= ~(1 << temp2);

	ssd1306_Frame[idx] = data;
}

void ssd1306_Draw_Line_H(uint8_t x_Start , uint8_t y_Start , uint8_t width , ssd1306_color_t color )
{
	/* Check if the width is pointing to negative direction and correct it*/
	/*Not Implemented*/

	/* Check if the coordinates is out of display*/
	if ((y_Start > 63) || (x_Start > 127)) return;

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
	uint8_t * data = &ssd1306_Frame[page * 128 + x_Start];

	for (; width > 0; width--)
	{
		if (color == ssd1306_color_White)
			*data |= bit;
		else *data &= bit;

		data++;
	}
}

void ssd1306_Draw_Line_V(uint8_t x_Start , uint8_t y_Start , uint8_t height ,
        ssd1306_color_t color )
{
	/* Check if the width is pointing to negative direction and correct it*/
	/*Not Implemented*/

	/* Check if the coordinates is out of display*/
	if ((y_Start > 63) || (x_Start > 127)) return;

	/*Check if the line runs outside the screen boundaries and crop if true*/
	if ((y_Start + height) > 64)
	{
		height = 64 - y_Start;
	}

	/* Get the Page and bit location*/
	uint8_t page = y_Start / 8;
	uint8_t bit = y_Start % 8; /* Indicates the bit location on scale of 0 ~ 7*/

	/* Get the location of the first byte in the frame buffer*/
	uint8_t * data = &ssd1306_Frame[page * 128 + x_Start];

	uint8_t bitMask = 0xFF;

	/* Test if the height of the line is still larger than the starting bit to the end of the byte*/
	while (height > 0)
	{
		if (height > (8 - bit))
		{
			bitMask <<= bit;
			if (color == ssd1306_color_White)
				*data |= bitMask;
			else *data &= ~bitMask;
			height -= (8 - bit);
			data += 128;
		}
		else
		{
			bitMask >>= (8 - height);
			bitMask <<= bit;
			if (color == ssd1306_color_White)
				*data |= bitMask;
			else *data &= ~bitMask;
			height = 0;

		}
		bit = 0;
		bitMask = 0xFF;
	}
}

void ssd1306_Draw_Line(uint8_t x0 , uint8_t y0 , uint8_t x1 , uint8_t y1 , ssd1306_color_t color )
{
	/* Check if any of the point is out of the display area and crop it*/
	if (x0 >= SSD1306_Display_Width) x0 = SSD1306_Display_Width - 1;
	if (x1 >= SSD1306_Display_Width) x1 = SSD1306_Display_Width - 1;
	if (y0 >= SSD1306_Display_Height) y0 = SSD1306_Display_Height - 1;
	if (y1 >= SSD1306_Display_Height) y1 = SSD1306_Display_Height - 1;

	int16_t dx;     //= (x1 - x0);
	int16_t dy;     //= -(y1 - y0);
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

void ssd1306_Draw_Direct_Rectangle(uint8_t x0 , uint8_t y0 , uint8_t x1 , uint8_t y1 ,
        ssd1306_color_t color )
{
	ssd1306_Draw_Line(x0, y0, x0, y1, color);
	ssd1306_Draw_Line(x0, y1, x1, y1, color);
	ssd1306_Draw_Line(x1, y0, x1, y1, color);
	ssd1306_Draw_Line(x0, y0, x1, y0, color);

}

void ssd1306_Draw_Recangle_Filled(uint8_t x0 , uint8_t y0 , uint8_t x1 , uint8_t y1 ,
        ssd1306_color_t color )
{
	if ((x0 > x1) | (y0 > y1)) return;

	uint8_t dx = x1 - x0;
	uint8_t dy = y1 - y0 + 1;

	for (uint8_t i = 0; i <= dx; i++)
	{
		ssd1306_Draw_Line_V(x0 + i, y0, dy, color);
	}
}

void ssd1306_Draw_Rectangle(ssd1306_point P0 , ssd1306_point P1 , ssd1306_point p2 ,
        ssd1306_point P3 , ssd1306_color_t color )
{
	ssd1306_Draw_Line(P0.x_point, P0.y_point, P1.x_point, P1.y_point, color);
	ssd1306_Draw_Line(P1.x_point, P1.y_point, p2.x_point, p2.y_point, color);
	ssd1306_Draw_Line(p2.x_point, p2.y_point, P3.x_point, P3.y_point, color);
	ssd1306_Draw_Line(P3.x_point, P3.y_point, P0.x_point, P0.y_point, color);
}

void ssd1306_Draw_Bitmap(uint8_t x_start , uint8_t y_start , const uint8_t *image , uint8_t width ,
        uint8_t height )
{
	/*
	 *  Removed as loop check it
	 *	if ((width == 0) | (height == 0))
	 *		return;
	 */
	uint8_t data = 0;
	uint8_t xbit = 0;
	uint8_t w;     //= width;
	uint8_t x;
	data = *image++;
	while (height > 0)
	{
		w = width;
		x = x_start;
		while (w > 0)
		{
			ssd1306_Draw_Pixel(x++, y_start,
			                   ((data & (1 << 7)) ? ssd1306_color_White : ssd1306_color_Black));
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

void ssd1306_Draw_Bitmap2(uint8_t x_start , uint8_t y_start , const uint8_t *image , uint8_t width ,
        uint8_t height , ssd1306_pixel_op operation )
{
	/*
	 *  Removed as loop check it
	 *	if ((width == 0) | (height == 0))
	 *		return;
	 */
	uint8_t data = 0;
	uint8_t xbit = 0;
	uint8_t w;     //= width;
	uint8_t x;
	data = *image++;
	while (height > 0)
	{
		w = width;
		x = x_start;
		while (w > 0)
		{
			ssd1306_Pixel_Set(x++, y_start, operation,
			                  ((data & (1 << 7)) ? ssd1306_color_White : ssd1306_color_Black));
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

