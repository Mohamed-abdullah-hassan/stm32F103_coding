//#include <stdio.h>
#include "stm32f103xb.h"
#include "LED.h"
#include "uart.h"
//#include "timebase.h"
#include "osKernel.h"



#define		QUANTA		10

typedef uint32_t TaskProfiler;

TaskProfiler Task0_Profiler,Task1_Profiler,Task2_Profiler,Task3_Profiler;

void motor_run(void);
void motor_stop(void);
void valve_open(void);
void valve_close(void);


void task0(void)
{
	while(1)
	{
		red_On();
		delay(5);
		Task0_Profiler++;
		red_Off();
		delay(50);
		red_On();
		delay(1);
		Task0_Profiler++;
		red_Off();
		delay(1);
		red_On();
		delay(1);
		Task0_Profiler++;
		red_Off();
		delay(1);
		red_On();
		delay(1);
		Task0_Profiler++;
		red_Off();
		delay(1);
		red_On();
		delay(1);
		Task0_Profiler++;
		red_Off();
		delay(1);
		red_On();
		delay(1);
		Task0_Profiler++;
		red_Off();
		delay(1);
		red_On();
		delay(1);
		Task0_Profiler++;
		red_Off();
		delay(1);
		red_On();
		delay(4);
		Task0_Profiler++;
		red_Off();
		delay(30);
		red_On();
		delay(30);
		Task0_Profiler++;
		red_Off();
		delay(100);
	}
}


void task1(void)
{
	while(1)
	{
		green_On();
		delay(50);
		Task1_Profiler++;
		green_Off();
		delay(50);
	}
}

void task2(void)
{
	while(1)
	{
		blue_On();
		delay(1);
		Task2_Profiler++;
		blue_Off();
		delay(99);
	}
}
/*Task 3 simulates a heavy process thats keep going*/
void task3 (void)
{
	while(1)
	{
		Task3_Profiler++;
	}
}


int main(void)
{
	led_Init();
	/*Initialize Kernel*/
	osKernelInit();
	/*Add Threads*/
	osKernelAddThreads(&task0, &task1, &task2, &task3);
	/*Set RoundRobin time quanta*/
	osKernelLaunch(QUANTA);
}
