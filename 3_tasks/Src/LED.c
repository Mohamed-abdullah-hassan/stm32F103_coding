#include "LED.h"

#define LED_GPIOB_EN	(1U<<3) /*GPIOC Enable bit in RCC_APB2ENR Register*/


void led_Init(void)
{
	/*The LED on the blue pill is connected to PC13 which is located in GPIOC port*/
	/*Enable GPIO Port C In ABD2ENR Register p.113 ref_man*/
	RCC->APB2ENR |= LED_GPIOB_EN;
	/*Set PC13 as Output pin in CRH register p.172 ref man*/
	/*CNF13 = 00 -> General Purpose output push/pull */
	/*MODE13 = 01 -> Output mode, max speed 10MHz*/
//	GPIOC->CRH &= ~((1<<23)|(1<<22)|(1<<20));
//	GPIOC->CRH |= (1<<21);
	GPIOB->CRH &= ~((1<<17)|(1<<18)|(1<<19)|(1<<21)|(1<<22)|(1<<23)|(1<<25)|(1<<26)|(1<<27));
	GPIOB->CRH |= ((1<<16)|(1<<20)|(1<<24));
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

void red_On(void)
{
	GPIOB->BSRR |= (1<<14);
}
void red_Off(void)
{
	GPIOB->BSRR |= (1<<30);
}

void green_On(void)
{
	GPIOB->BSRR |= (1<<13);
}
void green_Off(void)
{
	GPIOB->BSRR |= (1<<29);
}
void blue_On(void)
{
	GPIOB->BSRR |= (1<<12);
}
void blue_Off(void)
{
	GPIOB->BSRR |= (1<<28);
}

void led_Off(void)
{
	/*To Reset (OFF) PC13, Write 1 in reset bit for it location 29 */
	/*Writing 0 has no effect on pin state*/
//	GPIOC->BSRR |= (1<<29);

}
