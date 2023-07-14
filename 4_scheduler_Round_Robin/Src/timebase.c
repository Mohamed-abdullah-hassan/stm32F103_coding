#include "timebase.h"
#include "stm32f103xb.h"

//
//volatile uint32_t g_curr_tick;
//
//#define MAX_DELAY		0xFFFFFFFFU
//
//volatile uint32_t tick_freq = 1;
//
//uint32_t get_tick(void);
//void tick_increment(void);
//void delay(uint32_t delay);
//
//void timebase_init(void)
//{
//	SysTick->LOAD = 8000 -1;
//	SysTick->VAL	= 0;
//	SysTick->CTRL	= 1U<<2;
//	SysTick->CTRL	|= 1U<<1;
//	SysTick->CTRL	|= 1U<<0;
//
//	__enable_irq();
//}
//
////
////void SysTick_Handler(void)
////{
////	tick_increment();
////}
//
//void tick_increment(void)
//{
//	g_curr_tick += tick_freq;
////
//}
//void delay(uint32_t delay)
//{
//	uint32_t wait =  delay + get_tick();
//	while((get_tick()) < wait){}
//
//
//}
//uint32_t get_tick(void)
//{
//	__disable_irq();
//	uint32_t g_curr_tick_p = g_curr_tick;
//	__enable_irq();
//	return g_curr_tick_p;
//}
