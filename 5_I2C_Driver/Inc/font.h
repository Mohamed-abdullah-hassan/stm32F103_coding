/*
 * font.h
 *
 *  Created on: Jul 23, 2023
 *      Author: engmo
 */

#ifndef FONT_H_
#define FONT_H_

#include "stdint.h"

enum __font_Layout
{
	vertical_LSB_UP = 0x00,
	vertical_MSB_UP,
	horizontal_MSB_LEFT,
	horizontal_LSB_LEFT,
};

typedef uint8_t fontLayout;

typedef struct
{
	uint8_t charWidth;
	uint8_t charHeigh;
	const uint8_t *font;
	uint8_t count;
	uint8_t charSpace;
	fontLayout layOut;
} fontHead_t;

extern fontHead_t font_5x7;
extern fontHead_t font_7x10;
extern fontHead_t font_11x18;
extern fontHead_t font_16x26 ;
#endif /* FONT_H_ */
