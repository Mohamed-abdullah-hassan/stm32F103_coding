/*
 * ssd1306.h
 *
 *  Created on: Jul 15, 2023
 *      Author: engmo
 */

#ifndef SSD1306_H_
#define SSD1306_H__
#include "stdint.h"
#include "font.h"

#define SSD1306_Display_Height		64
#define SSD1306_Display_Pages		SSD1306_Display_Height/8
#define SSD1306_Display_Width		128

typedef enum
{
	ssd1306_color_Black = 0x00, ssd1306_color_White = 0xFF
}ssd1306_color_t;

typedef enum //ssd1306_pixel_operation
{
	ssd1306_pixel_Set = 0x00, ssd1306_pixel_AND, ssd1306_pixel_OR, ssd1306_pixel_XOR
}ssd1306_pixel_op;


typedef enum //ssd1306 Circle Draw quarters To define which quarter of the circle to draw
{
	ssd1306_circle_quarter_top_left  = 0x01,		// Draw only the upper left Quarter
	ssd1306_circle_quarter_top_right = 0x02,		// Draw only the upper Right Quarter
	ssd1306_circle_quarter_bot_Left  = 0x04,		// Draw only the Lower left Quarter
	ssd1306_circle_quarter_bot_right = 0x08,		// Draw only the Lower Right Quarter
	ssd1306_circle_half_ver_left	 = 0x05,		// Draw only the Vertical Left Half
	ssd1306_circle_half_ver_right	 = 0x0A,		// Draw only the Vertical Right Half
	ssd1306_circle_half_hor_top		 = 0x03,		// Draw only the Horizontal top Half
	ssd1306_circle_half_hor_bot		 = 0x0C,		// Draw only the Horizontal Bottom Half
	ssd1306_circle_full 			 = 0xFF,		// Draw Full circle
}ssd1306_circle_corners;

//typedef uint8_t ssd1306_circle_corners;


typedef struct
{
	uint8_t x_point;
	uint8_t y_point;
} ssd1306_point;

//static uint8_t ssd1306_Frame[SSD1306_Display_Pages * SSD1306_Display_Width];
//#define Frame_buffer	ssd1306_Frame
#define check_buffer_size	256



void ssd1306_I2C_Init( );

void ssd1306_I2C_Clear(void );
void ssd1306_I2C_Set(void );
void ssd1306_I2C_Fill(uint8_t pattern );
void ssd1306_I2C_Fill_char(uint8_t pattern );


void ssd1306_Reset_boundaries( );
void ssd1306_Directframe_Set(uint16_t indx , uint8_t data );
uint8_t ssd1306_Directframe_Get(uint16_t indx);
void ssd1306_Write_Partial_Frame( );
void ssd1306_Clear_Frame( );
void ssd1306_Fill_buffer(uint8_t pattern );

void ssd1306_Draw_Pixel(uint8_t x_Pos , uint8_t y_Pos , ssd1306_color_t color );

void ssd1306_Draw_Line_H(uint8_t x_Start , uint8_t y_Start , uint8_t width , ssd1306_color_t color );
void ssd1306_Draw_Line_V(uint8_t x_Start , uint8_t y_Start , uint8_t height , ssd1306_color_t color );
void ssd1306_Draw_Line(uint8_t x0 , uint8_t y0 , uint8_t x1 , uint8_t y1 , ssd1306_color_t color );

void ssd1306_Draw_Direct_Rectangle(uint8_t x0 , uint8_t y0 , uint8_t x1 , uint8_t y1 , ssd1306_color_t color );
void ssd1306_Draw_Recangle_Filled(uint8_t x0 , uint8_t y0 , uint8_t x1 , uint8_t y1 , ssd1306_color_t color );
void ssd1306_Draw_Rect_Round(uint8_t x_Pos, uint8_t y_Pos, uint8_t width, uint8_t height, uint8_t radius, ssd1306_circle_corners corners, ssd1306_color_t color );
void ssd1306_Draw_Rect_Round_filled(uint8_t x_Pos, uint8_t y_Pos, uint8_t width, uint8_t height, uint8_t radius, ssd1306_circle_corners corners, ssd1306_color_t color );

void ssd1306_Draw_Quadrila(ssd1306_point P0 , ssd1306_point P1 , ssd1306_point p2 , ssd1306_point P3 , ssd1306_color_t color );

void ssd1306_Draw_Bitmap(uint8_t x_start , uint8_t y_start , const uint8_t *image , uint8_t width , uint8_t height );
void ssd1306_Draw_Bitmap_PO(uint8_t x_start , uint8_t y_start , const uint8_t *image , uint8_t width , uint8_t height , ssd1306_pixel_op operation );

void ssd1306_Set_Cursor(uint8_t xPos , uint8_t yPos );
void ssd1306_Set_Font(fontHead_t *newFont );
void ssd1306_PutC(char c );
void ssd1306_PutSTR(char *str );


void ssd1306_Draw_Circle(uint16_t x_Pos, uint16_t y_Pos, uint16_t radius, ssd1306_color_t color, ssd1306_circle_corners corner);
void ssd1306_Draw_Circle_Filled(uint16_t x_Pos, uint16_t y_Pos, uint16_t radius, ssd1306_color_t color, ssd1306_circle_corners corner);

#endif /* SSD1306_H_ */
