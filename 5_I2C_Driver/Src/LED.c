#include "LED.h"
#include "gpio.h"
#include "system.h"

//#define LED_GPIOB_EN	(1U<<3) /*GPIOC Enable bit in RCC_APB2ENR Register*/

gpio_Pin_t LedRed;
gpio_Pin_t LedGreen;
gpio_Pin_t LedBlue;

void led_Init(void)
{

//	GPIO_Pin pin;
	LedRed.pin_Port   = Peripherals_GpioB;
	LedRed.pin_Mode   = pinMode_Output50MHz;
	LedRed.pin_Config = pinConfig_OutputPushPull;

	LedGreen = LedBlue  = LedRed;
	LedRed.pin_Number   = 14;
	LedGreen.pin_Number = 13;
	LedBlue.pin_Number  = 12;

	gpio_PinInit (&LedRed);
	gpio_PinInit (&LedGreen);
	gpio_PinInit (&LedBlue);
//
//	/*The LEDs on the blue pill are connected to PB14,PB13,PB12 which is located in GPIOB port*/
//	/*Enable GPIO Port B In ABD2ENR Register p.113 ref_man*/
//	RCC->APB2ENR |= LED_GPIOB_EN;
//	/*Set PB14,PB13,PB12 as Output pin in CRH register p.172 ref man*/
//	/*CNFxx = 00 -> General Purpose output push/pull */
//	/*MODExx = 01 -> Output mode, max speed 10MHz*/
//	GPIOB->CRH &= ~((1<<17)|(1<<18)|(1<<19)|(1<<21)|(1<<22)|(1<<23)|(1<<25)|(1<<26)|(1<<27));
//	GPIOB->CRH |= ((1<<16)|(1<<20)|(1<<24));
}


void led_On(void)
{
	/*Set PC13 On and Off Using BSRR register p.173*/
	/*Using BSRR instead of using ODR for atomic operation*/
	/*As using ODR requires the cycle of Read Modify Store to change the state of the bit*/
	/*While BSRR has two bits (SET, Reset) for each pin*/
	/*To set (ON) PC13 write 1 in position 13 of the register*/
//	GPIOC->BSRR |= (1<<13);
}

inline void red_On(void)
{
	gpio_SetPinHigh(&LedRed);
//	GPIOB->BSRR |= (1<<14);
}
inline void red_Off(void)
{
    gpio_SetPinLow(&LedRed);
//	GPIOB->BSRR |= (1<<30);
}

inline void green_On(void)
{
    gpio_SetPinHigh(&LedGreen);
//	GPIOB->BSRR |= (1<<13);
}
inline void green_Off(void)
{
    gpio_SetPinLow(&LedGreen);
//	GPIOB->BSRR |= (1<<29);
}
inline void blue_On(void)
{
    gpio_SetPinHigh(&LedBlue);
//	GPIOB->BSRR |= (1<<12);
}
inline void blue_Off(void)
{
    gpio_SetPinLow(&LedBlue);
//	GPIOB->BSRR |= (1<<28);
}

inline void led_Off(void)
{
	/*To Reset (OFF) PC13, Write 1 in reset bit for it location 29 */
	/*Writing 0 has no effect on pin state*/
//	GPIOC->BSRR |= (1<<29);

}
