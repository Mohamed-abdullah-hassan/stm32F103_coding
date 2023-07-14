#include "I2C.h"

#include "LED.h"

#include <stm32f103xb.h>

#define I2C_DEBUG_USING_LED

void i2c1_Init()
{
	/*Enable clock to i2c1 module on APB1 Bus*/
	RCC->APB1ENR |= (1 << 21);

	/*Initiate I2C peripheral reset*/
	I2C1->CR1 |= (1 << 15);
	I2C1->CR1 &= ~(1 << 15);

	I2C1->CR2 = 8; /* The APB1 bus clock value which is 8MHz*/
	I2C1->CCR |= 40; /* for 100 KHz standard mode*/
	I2C1->OAR1 = 0x4000; /*From page 776 bit 14 must be 1 */

	I2C1->TRISE = 9;
	I2C1->CR1 |= I2C_CR1_ACK; /*Enable ACK bit*/

	/*Enable clock for port B to use PB6 & PB7 as SCL & SDA on APB2 bus*/
	RCC->APB2ENR |= (1 << 3);

	/*Configure GPIO port pins for Open drain output alternate function with maximum speed*/
	GPIOB->CRL |= 0xFF << 24;

	/*If no external pull-up resistors are present enable ODR bit for the corresponding pins*/

	/*Enable the I2C peripheral */
	I2C1->CR1 |= I2C_CR1_PE;
}

void i2c1_Write_Begin(uint8_t s_address, uint8_t data)
{
	volatile register uint8_t temp;
	/*Set Start bit (bit 8) in CR1 */
	/*to Generate a start condition*/
	I2C1->CR1 |= (1 << 8ul);

	/*Check if SB(bit 0) in SR1 is set which means that a start condition is generated*/
	while (!(I2C1->SR1 & 0x01))
#ifdef I2C_DEBUG_USING_LED
		blue_On()	;
	blue_Off();
#else
	;
#endif

	/*write the salve address in DR register */
	I2C1->DR = s_address;

	/*Check if ADDR (bit 1) is set in SR1 */
	/*Then its followed by reading SR2*/
	while (!(I2C1->SR1 & 0x02))
#ifdef I2C_DEBUG_USING_LED
		green_On();
#else
	;
#endif
	temp = I2C1->SR2;
#ifdef I2C_DEBUG_USING_LED
	green_Off();
#endif

	/*Write data to DR register*/
	I2C1->DR = data;

	/*Check if TXE (bit 7) in SR1 is set*/

#ifdef I2C_DEBUG_USING_LED
	red_On();
#endif
	while (!(I2C1->SR1 & (1 << 7)))
		;
#ifdef I2C_DEBUG_USING_LED
	red_Off();
#endif


	/*Return*/
	return;

}

void i2c1_write(uint8_t data)
{
	/*Write data buffer to DR register*/
	I2C1->DR = data;

	/*Check if TXE bit in SR1 is set*/
	while (!(I2C1->SR1 & (1 << 7)))
		;

	/*Return*/
}

void i2c1_write_buff(uint8_t *data, uint16_t count)
{
	while (count > 0)
	{
		/*Check if TXE bit in SR1 is set*/
		while (!(I2C1->SR1 & (1 < 7)))
			;

		/*Write data buffer to DR register*/
		I2C1->DR = *data++;
		count--;
	}

	/*Check if TXE bit in SR1 is set*/
	while (!(I2C1->SR1 & (1 < 7)))
		;

	/*Return*/
}

void i2c1_End()
{
	/*Set Stop (bit 9) in CR1 register to send a stop condition*/
	I2C1->CR1 |= (1 << 9);
}
