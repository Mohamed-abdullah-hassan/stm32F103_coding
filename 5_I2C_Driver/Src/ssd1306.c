#include "I2C.h"
// #include "string.h"
#include "ssd1306.h"

/*       OLED Display routine
 * SSD1306 commands moving to seperate file after testing
 */
#define SSD1306_I2C_ADDR 0x78
#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A
#define SSD1306_DEACTIVATE_SCROLL 0x2E		  // Stop scroll
#define SSD1306_ACTIVATE_SCROLL 0x2F		  // Start scroll
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3 // Set scroll range
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7

/*
 * SSD1306 Display Macros
 */
/* Write command */
#define SSD1306_WRITECOMMAND(command) ssd1306_I2C_Write(SSD1306_I2C_ADDR, 0x00, (command))
/* Write data */
#define SSD1306_WRITEDATA(data) ssd1306_I2C_Write(SSD1306_I2C_ADDR, 0x40, (data))

/*
 * SSD1306 variables
 */
struct __display_Buffer
{
	uint8_t y_Page_Start;
	uint8_t y_Page_End;
	uint8_t x_Col_Start;
	uint8_t x_Col_End;
	uint8_t Frame[SSD1306_Display_Pages * SSD1306_Display_Width];
} display_Buffer;

struct __cursor
{
	uint8_t x_text;
	uint8_t y_text;
} textCursor;

fontHead_t * font;

const uint8_t ssd1306_init[] =
	{ (0xAE),     // display off
	  (0x20),     // Set Memory Addressing Mode
	  (0x10),     // 00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	  (0xB0),     // Set Page Start Address for Page Addressing Mode,0-7
	  (0xC8),     // Set COM Output Scan Direction
	  (0x00),     //---set low column address 0x00
	  (0x10),     //---set high column address 0x10
	  (0x40),     //--set start line address
	  (0x81),     //--set contrast control register
	  (0x01),     // Contrast Value from 0x00 to 0xFF
	  (0xA1),     //--set segment re-map 0 to 127
	  (0xA6),     //--set normal display
	  (0xA8),     //--set multiplex ratio(1 to 64)
	  (0x3F),     // 0x1f for 128*32
	  (0xA4),     // 0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	  (0xD3),     //-set display offset
	  (0x00),     //-not offset
	  (0xD5),     //--set display clock divide ratio/oscillator frequency
	  (0xF0),     //--set divide ratio
	  (0xD9),     //--set pre-charge period
	  (0x22),     //
	  (0xDA),     //--set com pins hardware configuration
	  (0x12),     //
	  (0xDB),     //--set vcomh
	  (0x20),     // 0x20,0.77xVcc
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
	textCursor.x_text = 0;
	textCursor.y_text = 0;
}

void ssd1306_I2C_Write(uint8_t address , uint8_t reg , uint8_t data )
{
	i2c1_write(data);
}

void ssd1306_Update_frame_XPage_boundaries(uint8_t x_Pos , uint8_t page )
{
	if (x_Pos >= SSD1306_Display_Width) x_Pos = SSD1306_Display_Width - 1;
	if (page >= SSD1306_Display_Pages) page = SSD1306_Display_Pages - 1;
	if (display_Buffer.x_Col_Start > x_Pos) display_Buffer.x_Col_Start = x_Pos;
	if (display_Buffer.y_Page_Start > page) display_Buffer.y_Page_Start = page;
	if (display_Buffer.x_Col_End < x_Pos) display_Buffer.x_Col_End = x_Pos;
	if (display_Buffer.y_Page_End < page) display_Buffer.y_Page_End = page;
}



void ssd1306_Update_frame_XY_boundaries(uint8_t x_Pos , uint8_t y_Pos )
{
	if (y_Pos >= SSD1306_Display_Height) y_Pos = SSD1306_Display_Height - 1;
	uint8_t page = (uint8_t) (y_Pos / 8);
	ssd1306_Update_frame_XPage_boundaries(x_Pos, page);
}

void ssd1306_Reset_boundaries()
{
	display_Buffer.x_Col_End = 0x00;
	display_Buffer.y_Page_End = 0x00;
	display_Buffer.x_Col_Start = SSD1306_Display_Height - 1;
	display_Buffer.y_Page_Start = SSD1306_Display_Width - 1;
}

void ssd1306_Directframe_Set(uint16_t indx , uint8_t data )
{
	display_Buffer.Frame[indx] = data;
	ssd1306_Update_frame_XPage_boundaries(indx % SSD1306_Display_Width,
	                                      indx / SSD1306_Display_Height);
}

void ssd1306_Pixel_Set(uint8_t x_Pos , uint8_t y_Pos , ssd1306_pixel_op operation , ssd1306_color_t color )
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
		else
			display_Buffer.Frame[(page * 128) + x_Pos] &= ~(temp);
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
		else
			new ^= 0x00;
		display_Buffer.Frame[(page * 128) + x_Pos] = ((display_Buffer.Frame[(page * 128) + x_Pos]
		        & (~temp))
		                                              | new);
	}
	ssd1306_Update_frame_XPage_boundaries(x_Pos, page);
}

void ssd1306_Write_Partial_Frame( )
{
	uint8_t m;
	uint16_t idx = 0;

	for (m = display_Buffer.y_Page_Start; m <= display_Buffer.y_Page_End; m++)
	{
		i2c1_Write_Begin(SSD1306_I2C_ADDR, 0x00);
		i2c1_write(0xB0 + m);
		i2c1_write(0x00 | ((display_Buffer.x_Col_Start) & 0x0F));
		i2c1_write(0x10 | ((display_Buffer.x_Col_Start >> 4)));
		i2c1_End();
		i2c1_Write_Begin(SSD1306_I2C_ADDR, 0x40);
		for (uint8_t i = display_Buffer.x_Col_Start; i <= display_Buffer.x_Col_End; i++)
		{
			idx = (128 * m) + i;
			i2c1_write(display_Buffer.Frame[idx]);
		}
		i2c1_End();
	}
	display_Buffer.x_Col_End = 0x00;
	display_Buffer.y_Page_End = 0x00;
	display_Buffer.x_Col_Start = SSD1306_Display_Height - 1;
	display_Buffer.y_Page_Start = SSD1306_Display_Width - 1;
}

void ssd1306_Clear_Frame( )
{
	for (uint16_t i = 0; i < sizeof(display_Buffer.Frame); i++)
	{
		display_Buffer.Frame[i] = 0x00;
	}
	display_Buffer.x_Col_End = SSD1306_Display_Width - 1;
	display_Buffer.y_Page_End = SSD1306_Display_Pages - 1;
	display_Buffer.x_Col_Start = 0x00;
	display_Buffer.y_Page_Start = 0x00;
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
	data = display_Buffer.Frame[idx];

	/* Calculate the bit number of the pixel in the byte*/
	uint8_t temp2 = (y_Pos % 8);

	/*Set or clear the pixel according to color*/
	if (color == ssd1306_color_White) /*Set the pixel*/
		data |= (1 << temp2);
	else
		data &= ~(1 << temp2);

	display_Buffer.Frame[idx] = data;
	ssd1306_Update_frame_XY_boundaries(x_Pos, y_Pos);
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
	//	uint8_t * data = &ssd1306_Frame[page * 128 + x_Start];
	uint8_t * data = &display_Buffer.Frame[page * 128 + x_Start];

	ssd1306_Update_frame_XY_boundaries(x_Start, y_Start);
	ssd1306_Update_frame_XY_boundaries(x_Start + width, y_Start);
	for (; width > 0; width--)
	{
		if (color == ssd1306_color_White)
			*data |= bit;
		else
			*data &= bit;
		data++;
	}
}

void ssd1306_Draw_Line_V(uint8_t x_Start , uint8_t y_Start , uint8_t height , ssd1306_color_t color )
{
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
	uint8_t * data = &display_Buffer.Frame[page * 128 + x_Start];
	uint8_t bitMask = 0xFF;

	ssd1306_Update_frame_XY_boundaries(x_Start, y_Start);
	ssd1306_Update_frame_XY_boundaries(x_Start, y_Start + height);
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
		//		ssd1306_Draw_Pixel(x0, y0, color);
		ssd1306_Pixel_Set(x0, y0, ssd1306_pixel_Set, color);
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

void ssd1306_Draw_Direct_Rectangle(uint8_t x0 , uint8_t y0 , uint8_t x1 , uint8_t y1 , ssd1306_color_t color )
{
	ssd1306_Draw_Line(x0, y0, x0, y1, color);
	ssd1306_Draw_Line(x0, y1, x1, y1, color);
	ssd1306_Draw_Line(x1, y0, x1, y1, color);
	ssd1306_Draw_Line(x0, y0, x1, y0, color);
}

void ssd1306_Draw_Recangle_Filled(uint8_t x0 , uint8_t y0 , uint8_t x1 , uint8_t y1 , ssd1306_color_t color )
{
	if ((x0 > x1) | (y0 > y1)) return;
	uint8_t dx = x1 - x0;
	uint8_t dy = y1 - y0 + 1;
	for (uint8_t i = 0; i <= dx; i++)
	{
		ssd1306_Draw_Line_V(x0 + i, y0, dy, color);
	}
}

void ssd1306_Draw_Rectangle(ssd1306_point P0 , ssd1306_point P1 , ssd1306_point p2 , ssd1306_point P3 , ssd1306_color_t color )
{
	ssd1306_Draw_Line(P0.x_point, P0.y_point, P1.x_point, P1.y_point, color);
	ssd1306_Draw_Line(P1.x_point, P1.y_point, p2.x_point, p2.y_point, color);
	ssd1306_Draw_Line(p2.x_point, p2.y_point, P3.x_point, P3.y_point, color);
	ssd1306_Draw_Line(P3.x_point, P3.y_point, P0.x_point, P0.y_point, color);
}

void ssd1306_Draw_Bitmap(uint8_t x_start , uint8_t y_start , const uint8_t *image , uint8_t width , uint8_t height )
{
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

void ssd1306_Draw_Bitmap2(uint8_t x_start , uint8_t y_start , const uint8_t *image , uint8_t width , uint8_t height , ssd1306_pixel_op operation )
{
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

void ssd1306_Set_Cursor(uint8_t xPos , uint8_t yPos )
{
	if (xPos >= SSD1306_Display_Width) return;
	if (yPos >= SSD1306_Display_Height) return;
	textCursor.x_text = xPos;
	textCursor.y_text = yPos;
}

enum __cursor_step
{
	step_left = 0x00, step_Right, step_Down, step_Up, step_Most_left, step_Most_Up,
};
typedef uint8_t cursor_step;

void ssd1306_Step_Cursor(cursor_step dir )
{
	uint8_t value;
	if (dir == step_left)
	{
		value = font->charWidth + font->charSpace;
		textCursor.x_text += value;
		if (textCursor.x_text >= SSD1306_Display_Width)
		{
			ssd1306_Step_Cursor(step_Most_left);
			ssd1306_Step_Cursor(step_Down);
		}
	}
	else if (dir == step_Most_left)
	{
		textCursor.x_text = 0;
	}
	else if (dir == step_Most_Up)
	{
		textCursor.y_text = 0;
	}
	else if (dir == step_Down)
	{
		value = font->charHeigh + font->charSpace;
		textCursor.y_text += value;
		if (textCursor.y_text >= SSD1306_Display_Height)
		{
			ssd1306_Step_Cursor(step_Most_Up);
		}
	}
}

void ssd1306_Set_Font(fontHead_t *newFont )
{
	if (newFont) font = newFont;
}

void ssd1306_PutC(char c )
{
	// Check if there is a defined font
	if (!font) return;
	// Check if the character is special type
	if (c == '\n')
	{
		ssd1306_Step_Cursor(step_Down);
		ssd1306_Step_Cursor(step_Most_left);
		return;
	}
	if (c == '\r')     // Oddly printf doesn't send \r character
	{
		return;
	}

	// The start of printable characters in ASCII table begins with space
	// Check if the received character is printable
	if (c < ' ') return;
	// Subtracting (' ', 0x20, or 32) from the received char to define the position of the character in the table
	c -= 0x20;
	// The standard ASCII table contains 95 printable characters, but some fonts has some custom glyph at the end.
	// check if the character exist in the font
	if (c >= font->count) return;
	// The orientation of the font data is by rows "vertical"
	// Define the length of each row in Bytes
	uint8_t colBytes = ((uint8_t) (font->charHeigh - 1) / 8);     // number of bytes for each column
	// if the font height is (1-8) bits then the number of bytes is 1, and if its (9 - 16) bits then the number of Bytes is 2, and so on
	colBytes++;
	// Get the position of the Character first byte
	uint16_t idx;
	uint8_t data = 0;
	if ((textCursor.x_text + font->charWidth) >= SSD1306_Display_Width)
	{
		ssd1306_Step_Cursor(step_Most_left);
		ssd1306_Step_Cursor(step_Down);
	}

	if (font->layOut == vertical_LSB_UP)
	{
		uint16_t charSize = (colBytes * (font->charWidth));
		idx = charSize * c;
		for (uint8_t x = 0; x < font->charWidth; x++)
		{
			for (uint8_t y = 0; y < font->charHeigh; y++)
			{
				if ((y % 8) == 0) data = font->font[idx++];
				if (data & 0x01)
					ssd1306_Pixel_Set(textCursor.x_text + x, textCursor.y_text + y,
					                  ssd1306_pixel_Set, ssd1306_color_White);
				else
					ssd1306_Pixel_Set(textCursor.x_text + x, textCursor.y_text + y,
					                  ssd1306_pixel_Set, ssd1306_color_Black);
				data >>= 1;
			}
		}

	}
	else if (font->layOut == horizontal_LSB_LEFT)
	{
		uint8_t rowBytes = ((uint8_t) (font->charWidth - 1) / 8)+1;
		uint16_t charSize = (rowBytes * (font->charHeigh));
		idx = charSize * c;

		for (uint8_t i = 0; i < font->charHeigh; i++)
			for (uint8_t j = 0; j < font->charWidth; j++)
			{
				if ((j % 8) == 0) data = font->font[idx++];
				if (data & 0x80)
					ssd1306_Pixel_Set(textCursor.x_text + j, textCursor.y_text + i,
					                  ssd1306_pixel_Set, ssd1306_color_White);
				else
					ssd1306_Pixel_Set(textCursor.x_text + j, textCursor.y_text + i,
					                  ssd1306_pixel_Set, ssd1306_color_Black);
				data <<= 1;

			}
	}
	if (font->charSpace >0)
	{
		for (uint8_t j = 0; j< font->charSpace; j++)
		{
			for (uint8_t i = 0; i < font->charHeigh; i++)
			{
				ssd1306_Pixel_Set(textCursor.x_text + font->charWidth + j, textCursor.y_text + i,
									                  ssd1306_pixel_Set, ssd1306_color_Black);
				if(i< font->charWidth) ssd1306_Pixel_Set(textCursor.x_text + j, textCursor.y_text +font->charHeigh + i,
										                  ssd1306_pixel_Set, ssd1306_color_Black);
			}
		}

	}
	ssd1306_Step_Cursor(step_left);

}

void ssd1306_PutSTR(char *str)
{
	while (*str) // Every string has NULL character at the end
	{
		ssd1306_PutC(*str++);
	}
}

void ssd1306_Draw_Circle(uint16_t x_Pos, uint16_t y_Pos, uint16_t radius, ssd1306_color_t color, ssd1306_circle_corners corner)
{

	int16_t f = 1 - radius;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * radius;
	int16_t x = 0;
	int16_t y = radius;

	if ((corner & ssd1306_circle_half_hor_top))
	    ssd1306_Pixel_Set(x_Pos, y_Pos - radius, ssd1306_pixel_Set, color);

	if ((corner & ssd1306_circle_half_ver_right))
	    ssd1306_Pixel_Set(x_Pos + radius, y_Pos, ssd1306_pixel_Set, color);

	if ((corner & ssd1306_circle_half_ver_left))
	    ssd1306_Pixel_Set(x_Pos - radius, y_Pos, ssd1306_pixel_Set, color);

	if ((corner & ssd1306_circle_half_hor_bot))
	    ssd1306_Pixel_Set(x_Pos, y_Pos + radius, ssd1306_pixel_Set, color);


	while (x < y)
	{
		if (f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		if (corner & ssd1306_circle_quarter_bot_Left)
		{
			ssd1306_Pixel_Set(x_Pos - x, y_Pos + y, ssd1306_pixel_Set, color);     //right bot
//			ssd1306_Write_Partial_Frame();
//			delay2();
			ssd1306_Pixel_Set(x_Pos - y, y_Pos + x, ssd1306_pixel_Set, color);     // left bot
//			ssd1306_Write_Partial_Frame();
//							delay2();
		}
		if (corner & ssd1306_circle_quarter_bot_right)
		{
			ssd1306_Pixel_Set(x_Pos + x, y_Pos + y, ssd1306_pixel_Set, color);     //left bot
			ssd1306_Pixel_Set(x_Pos + y, y_Pos + x, ssd1306_pixel_Set, color);     // right bot
		}
		if (corner & ssd1306_circle_quarter_top_left)
		{
			ssd1306_Pixel_Set(x_Pos - y, y_Pos - x, ssd1306_pixel_Set, color);     // left top
			ssd1306_Pixel_Set(x_Pos - x, y_Pos - y, ssd1306_pixel_Set, color);     //right top
		}
		if (corner & ssd1306_circle_quarter_top_right)
		{
			ssd1306_Pixel_Set(x_Pos + x, y_Pos - y, ssd1306_pixel_Set, color);     //left top
			ssd1306_Pixel_Set(x_Pos + y, y_Pos - x, ssd1306_pixel_Set, color);     // right top
		}
	}
}
void ssd1306_Draw_Circle_Filled(uint16_t x_Pos, uint16_t y_Pos, uint16_t radius, ssd1306_color_t color, ssd1306_circle_corners corner)
{
	int16_t f = 1 - radius;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * radius;
	int16_t x = 0;
	int16_t y = radius;

	if ((corner & ssd1306_circle_half_hor_top))
	    ssd1306_Draw_Line_V(x_Pos, y_Pos - radius, radius, color);

	if ((corner & ssd1306_circle_half_ver_right)) ssd1306_Draw_Line_H(x_Pos, y_Pos, radius, color);

	if ((corner & ssd1306_circle_half_ver_left))
	    ssd1306_Draw_Line_H(x_Pos - radius, y_Pos, radius, color);

	if ((corner & ssd1306_circle_half_hor_bot)) ssd1306_Draw_Line_V(x_Pos, y_Pos, radius, color);

	while (x < y)
	{
		if (f >= 0)
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		if (corner & ssd1306_circle_quarter_bot_Left)
		{
//			ssd1306_Pixel_Set(x_Pos - x, y_Pos + y, ssd1306_pixel_Set, color);     //right bot
//			ssd1306_Pixel_Set(x_Pos - y, y_Pos + x, ssd1306_pixel_Set, color);     // left bot
//			ssd1306_Write_Partial_Frame();
//			delay2();
			ssd1306_Draw_Line_V(x_Pos - x, y_Pos, y + 1, color);
			ssd1306_Draw_Line_V(x_Pos - y, y_Pos, x + 1, color);
//				ssd1306_Write_Partial_Frame();
//				delay2();

		}
		if (corner & ssd1306_circle_quarter_bot_right)
		{
			ssd1306_Draw_Line_V(x_Pos + x, y_Pos, y + 1, color);
			ssd1306_Draw_Line_V(x_Pos + y, y_Pos, x + 1, color);
//				ssd1306_Pixel_Set(x_Pos + x, y_Pos + y, ssd1306_pixel_Set, color);     //left bot
//				ssd1306_Pixel_Set(x_Pos + y, y_Pos + x, ssd1306_pixel_Set, color);     // right bot
		}
		if (corner & ssd1306_circle_quarter_top_left)
		{
			ssd1306_Draw_Line_V(x_Pos - y, y_Pos - x, x + 1, color);
			ssd1306_Draw_Line_V(x_Pos - x, y_Pos - y, y + 1, color);
//				ssd1306_Pixel_Set(x_Pos - y, y_Pos - x, ssd1306_pixel_Set, color);     // left top
//				ssd1306_Pixel_Set(x_Pos - x, y_Pos - y, ssd1306_pixel_Set, color);     //right top
		}
		if (corner & ssd1306_circle_quarter_top_right)
		{
			ssd1306_Draw_Line_V(x_Pos + x, y_Pos - y, y + 1, color);
			ssd1306_Draw_Line_V(x_Pos + y, y_Pos - x, x + 1, color);
//				ssd1306_Pixel_Set(x_Pos + x, y_Pos - y, ssd1306_pixel_Set, color);     //left top
//				ssd1306_Pixel_Set(x_Pos + y, y_Pos - x, ssd1306_pixel_Set, color);     // right top
		}
	}
}

void ssd1306_Draw_Rect_Round(uint8_t x_Pos, uint8_t y_Pos, uint8_t width, uint8_t height, uint8_t radius, ssd1306_circle_corners corners, ssd1306_color_t color )
{
	uint8_t x1,x2,x3,x4,y1,y2,y3,y4;
	x1 = x2 = x_Pos;
	x3 = x4 = x_Pos + width;
	y1 = y2 = y_Pos;
	y3 = y4 = y_Pos + height;
	if(corners & ssd1306_circle_quarter_top_left)
	{
		x1 += radius;
		y1 += radius;
		ssd1306_Draw_Circle(x1, y1, radius, color, ssd1306_circle_quarter_top_left);
	}
	if(corners & ssd1306_circle_quarter_top_right)
	{
		x3 -= radius;
		y2 += radius;
		ssd1306_Draw_Circle(x3, y2, radius, color, ssd1306_circle_quarter_top_right);
	}
	if(corners & ssd1306_circle_quarter_bot_Left)
	{
		x2 += radius;
		y3 -= radius;
		ssd1306_Draw_Circle(x2, y3, radius, color, ssd1306_circle_quarter_bot_Left);
	}
	if(corners & ssd1306_circle_quarter_bot_right)
	{
		x4 -= radius;
		y4 -= radius;
		ssd1306_Draw_Circle(x4, y4, radius, color,ssd1306_circle_quarter_bot_right);
	}

	ssd1306_Draw_Line_H(x1, y_Pos, (x3 - x1), color);
	ssd1306_Draw_Line_V(x_Pos, y1, (y3 - y1), color);
	ssd1306_Draw_Line_H(x2, y_Pos+ height, (x4 - x2), color);
	ssd1306_Draw_Line_V(x_Pos + width, y2, (y4 - y2), color);

}

void ssd1306_Draw_Rect_Round_filled(uint8_t x_Pos, uint8_t y_Pos, uint8_t width, uint8_t height, uint8_t radius, ssd1306_circle_corners corners, ssd1306_color_t color )
{
//	if(width < (2 * radius)) radius = width >>1;
//	if (height < (2* radius)) radius = height >>1;
	if(width < (2 * radius)) return;
	if (height < (2* radius)) return;
	uint8_t x1,x2,x3,x4,y1,y2,y3,y4;
	x1 = x2 = x_Pos;
	x3 = x4 = x_Pos + width;
	y1 = y2 = y_Pos;
	y3 = y4 = y_Pos + height;
	if(corners & ssd1306_circle_quarter_top_left)
	{
		x1 += radius;
		y1 += radius;
		ssd1306_Draw_Circle_Filled(x1, y1, radius, color, ssd1306_circle_quarter_top_left);
	}
	if(corners & ssd1306_circle_quarter_top_right)
	{
		x3 -= radius;
		y2 += radius;
		ssd1306_Draw_Circle_Filled(x3, y2, radius, color, ssd1306_circle_quarter_top_right);
	}
	if(corners & ssd1306_circle_quarter_bot_Left)
	{
		x2 += radius;
		y3 -= radius;
		ssd1306_Draw_Circle_Filled(x2, y3, radius, color, ssd1306_circle_quarter_bot_Left);
	}
	if(corners & ssd1306_circle_quarter_bot_right)
	{
		x4 -= radius;
		y4 -= radius;
		ssd1306_Draw_Circle_Filled(x4, y4, radius, color,ssd1306_circle_quarter_bot_right);
	}

	uint8_t x_temp=0, y_temp=0;
	if(y2 >= y1)	y_temp = y2;
	else y_temp = y1;
	ssd1306_Draw_Recangle_Filled(x1, y_Pos, x3, y_temp, color);

	if(x2 >= x1) x_temp = x2;
	else x_temp = x1;
	ssd1306_Draw_Recangle_Filled(x_Pos, y1, x_temp, y3, color);

	if(x4 >= x3) x_temp = x3;
	else x_temp = x4;
	ssd1306_Draw_Recangle_Filled(x_temp, y2, x_Pos + width, y4, color);

	if(y4 >= y3)	y_temp = y3;
	else y_temp = y4;
	ssd1306_Draw_Recangle_Filled(x2, y_temp, x4, y_Pos + height, color);

	if(y2 >= y1)	y_temp = y1;
	else y_temp = y2;
	if(x1 >= x2) x_temp = x1;
	else x_temp = x2;
	if(x3 < x4) x4 = x3;
	if(y4 > y3)	y4 = y3;

	ssd1306_Draw_Recangle_Filled(x_temp, y_temp, x4, y4, color);

}
