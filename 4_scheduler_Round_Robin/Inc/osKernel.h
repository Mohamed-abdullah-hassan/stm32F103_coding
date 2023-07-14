#ifndef __OSKERNEL_H__
#define __OSKERNEL_H__
#include <stdint.h>
#include "stm32f103xb.h"

void osKernelInit(void);
void osKernelLaunch(uint32_t quanta);
//uint8_t osKernelAddThreads(void(*task0)(void),void(*task1)(void),void(*task2)(void));
uint8_t osKernelAddThreads(void(*task0)(void),void(*task1)(void),void(*task2)(void),void(*task3)(void));

void delay(uint32_t delay);

#endif




//#ifndef __OS_KERNEL_H__
//#define __OS_KERNEL_H__
//
//#include <stdint.h>
//
//void osKernelInit(void);
//void osKernelLaunch(uint32_t quanta);
//uint8_t osKernelAddThreads(void(*task0)(void),void(*task1)(void),void(*task2)(void),void(*task3)(void));
//
//
//uint32_t get_tick(void);
//void tick_increment(void);
//void delay(uint32_t delay);
//
//#endif /* OSKERNEL_H_ */
