#include <stm32f103xb.h>
#include "uart.h"

#define	usart_TX_GPIO_EN	(1U<<2)

void uart_TX_Init(void)
{
	/*            Initialize USART1            */

	/*Enable USART1 Clock*/
	RCC->APB2ENR |= (1<<14);
	/*Configure Baud rate */
	USART1->BRR = 0x45;
	/*Clear CR1 Register and Enable TX*/
	USART1->CR1	= USART_CR1_TE;
	/*Enable USART1*/
	USART1->CR1 |= USART_CR1_UE;

	/*Enable GPIO Port A*/
	RCC->APB2ENR |= usart_TX_GPIO_EN;
	/*Set PA9 as Alternate Function Output push/pull*/
	GPIOA->CRH &= ~((1<<6)| (1<<4));
	GPIOA->CRH	|= ((1<<7) | (1<<5) );
}


void uart_Send(unsigned char c)
{
	while (!(USART1->SR & USART_SR_TC)); //wait for TX to be complete
	USART1->DR = c;  //send it back out

}
