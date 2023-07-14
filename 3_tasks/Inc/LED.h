/*
 * LED.h
 *
 *  Created on: Apr 25, 2023
 *      Author: engmo
 */

#ifndef __LED_H_
#define __LED_H_

#include "stm32f103xb.h"

void led_Init(void);
void led_On(void);
void led_Off(void);
void red_On(void);
void red_Off(void);
void green_On(void);
void green_Off(void);
void blue_On(void);
void blue_Off(void);
#endif /* LED_H_ */
