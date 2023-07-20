/*
 * ssd1306.h
 *
 *  Created on: Jul 15, 2023
 *      Author: engmo
 */

#ifndef SSD1306_H_
#define SSD1306_H_

#include "stdint.h"

#define SSD1306_Display_Height		64
#define SSD1306_Display_Pages		SSD1306_Display_Height/8
#define SSD1306_Display_Width		128

enum ssd1306_color_pallet
{
	ssd1306_color_Black = 0x00, ssd1306_color_White = 0xFF
};
typedef uint8_t ssd1306_color_t;
enum ssd1306_pixel_operation
{
	ssd1306_pixel_Set = 0x00, ssd1306_pixel_AND, ssd1306_pixel_OR, ssd1306_pixel_XOR
};

typedef uint8_t ssd1306_pixel_op;

typedef struct
{
	uint8_t x_point;
	uint8_t y_point;
} ssd1306_point;

static uint8_t ssd1306_Frame[SSD1306_Display_Pages * SSD1306_Display_Width];
#define Frame_buffer	ssd1306_Frame
#define check_buffer_size	256

//typedef struct
//{
//	uint8_t buffer0[check_buffer_size];
//	uint8_t Frame[SSD1306_Display_Pages * SSD1306_Display_Width];
//	uint8_t buffer1[check_buffer_size];
//} ssd1306_buffer;
//
//ssd1306_buffer my_Frame;

void ssd1306_I2C_Init( );
void ssd1306_Clear_Frame( );
void ssd1306_I2C_Write(uint8_t address , uint8_t reg , uint8_t data );
void ssd1306_I2C_Write_Frame( );
void ssd1306_I2C_Clear(void );
void ssd1306_I2C_Set(void );
void ssd1306_I2C_Fill(uint8_t pattern );
void ssd1306_I2C_Fill_char(uint8_t pattern );
void ssd1306_Draw_Pixel(uint8_t x_Pos , uint8_t y_Pos , ssd1306_color_t color );
void ssd1306_Draw_Line_H(uint8_t x_Start , uint8_t y_Start , uint8_t width , ssd1306_color_t color );
void ssd1306_Draw_Line_V(uint8_t x_Start , uint8_t y_Start , uint8_t height ,
        ssd1306_color_t color );
void ssd1306_Draw_Line(uint8_t x0 , uint8_t y0 , uint8_t x1 , uint8_t y1 , ssd1306_color_t color );
void ssd1306_Draw_Direct_Rectangle(uint8_t x0 , uint8_t y0 , uint8_t x1 , uint8_t y1 ,
        ssd1306_color_t color );
void ssd1306_Draw_Recangle_Filled(uint8_t x0 , uint8_t y0 , uint8_t x1 , uint8_t y1 ,
        ssd1306_color_t color );
void ssd1306_Draw_Rectangle(ssd1306_point P0 , ssd1306_point P1 , ssd1306_point p2 ,
        ssd1306_point P3 , ssd1306_color_t color );
void ssd1306_Draw_Bitmap(uint8_t x_start , uint8_t y_start , const uint8_t *image , uint8_t width ,
        uint8_t height );
void ssd1306_Draw_Bitmap2(uint8_t x_start , uint8_t y_start , const uint8_t *image , uint8_t width ,
        uint8_t height, ssd1306_pixel_op operation );
void ssd1306_Fill_buffer(uint8_t pattern );
void ssd1306_Write_Partial_Frame( );

#endif /* SSD1306_H_ */
