#include "osKernel.h"
#include "stm32f103xb.h"

#define NUM_OF_THREADS			4
#define STACK_SIZE				30 /*Each Task (or thread) will have 30 word of stack means 120 byte for each*/

volatile uint32_t g_curr_tick,g_curr_tick_verifiy;

//volatile uint32_t tick_freq = 1;

typedef struct tcb{
	uint32_t *stackPt;
	struct tcb *nextPt;
}tcbType;

tcbType tcbs[NUM_OF_THREADS];
tcbType *currentPt;

uint32_t tcb_Stack[NUM_OF_THREADS][STACK_SIZE];

void osSchedulerLaunch(void);
uint32_t get_tick(void);

void osKernelStackInit(int i)
{
	tcbs[i].stackPt = &tcb_Stack[i][STACK_SIZE - 16]; /*Stack Pointer*/
	tcb_Stack[i][STACK_SIZE -1] = (1U<<24); /*PSR register set in Thumb mode*/
	/*tcb_Stack[i][STACK_SIZE -2] = ????;     The value of PC isn't known so skipping*/
	tcb_Stack[i][STACK_SIZE -3] = 0XAAAAAAAA;  /*LR*/
	tcb_Stack[i][STACK_SIZE -4] = 0XAAAAAAAA;  /*R12*/
	tcb_Stack[i][STACK_SIZE -5] = 0XAAAAAAAA;  /*R3*/
	tcb_Stack[i][STACK_SIZE -6] = 0XAAAAAAAA;  /*R2*/
	tcb_Stack[i][STACK_SIZE -7] = 0XAAAAAAAA;  /*R1*/
	tcb_Stack[i][STACK_SIZE -8] = 0XAAAAAAAA;  /*R0*/
	tcb_Stack[i][STACK_SIZE -9] = 0XAAAAAAAA;  /*R11*/
	tcb_Stack[i][STACK_SIZE -10] = 0XAAAAAAAA;  /*R10*/
	tcb_Stack[i][STACK_SIZE -11] = 0XAAAAAAAA;  /*R9*/
	tcb_Stack[i][STACK_SIZE -12] = 0XAAAAAAAA;  /*R8*/
	tcb_Stack[i][STACK_SIZE -13] = 0XAAAAAAAA;  /*R7*/
	tcb_Stack[i][STACK_SIZE -14] = 0XAAAAAAAA;  /*R6*/
	tcb_Stack[i][STACK_SIZE -15] = 0XAAAAAAAA;  /*R5*/
	tcb_Stack[i][STACK_SIZE -16] = 0XAAAAAAAA;  /*R4*/
}

uint8_t osKernelAddThreads(void(*task0)(void),void(*task1)(void),void(*task2)(void),void(*task3)(void))
{
	/*Disable Global interrupts*/
	__disable_irq();
	tcbs[0].nextPt = &tcbs[1];
	tcbs[1].nextPt = &tcbs[2];
	tcbs[2].nextPt = &tcbs[3];
	tcbs[3].nextPt = &tcbs[0];

	osKernelStackInit(0);
	tcb_Stack[0][STACK_SIZE -2] = (uint32_t)(task0);

	osKernelStackInit(1);
	tcb_Stack[1][STACK_SIZE -2] = (uint32_t)(task1);

	osKernelStackInit(2);
	tcb_Stack[2][STACK_SIZE -2] = (uint32_t)(task2);

	osKernelStackInit(3);
	tcb_Stack[3][STACK_SIZE -2] = (uint32_t)(task3);

	currentPt = &tcbs[0];
	__enable_irq();
	return 1;
}

void osKernelInit(void)
{
}

void osKernelLaunch(uint32_t quanta)
{
	SysTick->CTRL	= 0;
	SysTick->VAL	= 0;
	SysTick->LOAD	= (quanta * 8000 ) -1;
	NVIC_SetPriority(SysTick_IRQn,15);
	SysTick->CTRL	= 1U<<2;
	SysTick->CTRL	|= 1U<<1;
	SysTick->CTRL	|= 1U<<0;
	osSchedulerLaunch();
}
__attribute__((naked)) void SysTick_Handler(void)
{
	__asm("CPSID	I");
	__asm("LDR		R0,=g_curr_tick");
	__asm("LDR		R1,[R0]");
	__asm("ADD		R1,R1,#1");
	__asm("STR		R1,[R0]");
	__asm("LDR		R0,=g_curr_tick_verifiy");
	__asm("STR		R1,[R0]");

	__asm("PUSH 	{R4-R11}");
	__asm("LDR 		R0, =currentPt");
	__asm("LDR		R1,[R0]");
	__asm("STR 		SP,[R1]");
	__asm("LDR 		R1,[R1,#4]");
	__asm("STR		R1,[R0]");
	__asm("LDR 		SP,[R1]");
	__asm("POP		{R4-R11}");
	__asm("CPSIE  	I");
	__asm("BX		LR");

}

void osSchedulerLaunch(void)
{
	__asm("LDR    R0, =currentPt");
	__asm("LDR    R2,[R0]");
	__asm("LDR    SP,[R2]");
	__asm("POP    {R4-R11}");
	__asm("POP    {R12}");
	__asm("POP    {R0-R3}");
	__asm("ADD    SP,SP,#4");
	__asm("POP    {LR}");
	__asm("ADD    SP,SP,#4");
	__asm("CPSIE  	I");
	__asm("BX		LR");
}


void delay(uint32_t delay)
{
	uint32_t wait =  delay + get_tick();
	while((get_tick()) < wait){}
}

/* get_tick() returns the tick counter(g_curr_tick) which is updated inside SysTick timer interrupt
 * this method uses another variable g_curr_tick_verifiy to check if the SysTick interrupt occurred
 * and updated the variable while processing "detects data corruption", using this method Eliminates
 *  __disable_irq() function within running tasks*/
uint32_t get_tick(void)
{

	uint32_t g_curr_tick_p;
	//	__disable_irq();
	while(g_curr_tick_p !=g_curr_tick_verifiy)
	{
		g_curr_tick_p = g_curr_tick;
	}
//	__enable_irq();
	return g_curr_tick_p;
}
