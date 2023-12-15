#include <oI2C.h>
#include "LED.h"

#include <stm32f103xb.h>
#include <system.h>
#include "gpio.h"
#include "bit.h" /* for bit manipulation*/




#define I2C_DEBUG_USING_LED

void i2c1_Init()
{
	/*Enable clock to i2c1 module on APB1 Bus*/
//	RCC->APB1ENR |= (1 << 21);
	system_PeripheralEnable(Peripherals_I2c1);

	/*Initiate I2C peripheral reset*/
	I2C1->CR1 |= (1 << 15);
	I2C1->CR1 &= ~(1 << 15);

	I2C1->CR2 = 8; /* The APB1 bus clock value which is 8MHz*/
	I2C1->CCR |= 40; /* for 100 KHz standard mode*/
	I2C1->OAR1 = 0x4000; /*From page 776 bit 14 must be 1 */

	I2C1->TRISE = 9;
	I2C1->CR1 |= I2C_CR1_ACK; /*Enable ACK bit*/

	/*Enable clock for port B to use PB6 & PB7 as SCL & SDA on APB2 bus*/
	/*Configure GPIO port pins for Open drain output alternate function with maximum speed*/
	/*If no external pull-up resistors are present enable ODR bit for the corresponding pins*/
	gpio_Pin_t pin;
	pin.pin_Port   = Peripherals_GpioB;
	pin.pin_Number = 6;
	pin.pin_Config = pinConfig_AltereFuncOutputOpenDrain;
	pin.pin_Mode   = pinMode_Output50MHz;

	gpio_PinInit(&pin);
	pin.pin_Number = 7;
	gpio_PinInit(&pin);
//	RCC->APB2ENR |= (1 << 3);
//	GPIOB->CRL |= 0xFF << 24;


	/*Enable the I2C peripheral */
	I2C1->CR1 |= I2C_CR1_PE;
}

void i2c_Begin(uint8_t address)
{
	/*Generate a start condition*/

	I2C1->CR1 |= I2C_CR1_START ;

	/*Check if SB(bit 0) in SR1 is set which means that a start condition is generated*/
	while (!(I2C1->SR1 & I2C_SR1_SB))
#ifdef I2C_DEBUG_USING_LED
		blue_On();
	blue_Off();
#else
	;
#endif

	/*write the salve address in DR register */
	I2C1->DR = address;

	/*Check if ADDR (bit 1) is set in SR1, which means Slave ACK the address*/
	/*Then its followed by reading SR2*/
	/*TODO: Check if NACK is received*/
	while (!(I2C1->SR1 & I2C_SR1_ADDR))
#ifdef I2C_DEBUG_USING_LED
		green_On();
#else
		;
	#endif

	address = I2C1->SR2; /* using any unused variable to read SR2 and prevent the compiler form issuing a warning*/

}

void i2c1_Write_Begin(uint8_t s_address, uint8_t data)
{
	i2c_Begin(s_address);

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

void i2c_Read_Begin(uint8_t address, uint8_t *data)
{
	i2c_Begin(address);
	I2C1->CR1 |= I2C_CR1_STOP;
#ifdef I2C_DEBUG_USING_LED
	red_On();
#endif
	while (!(I2C1->SR1 & I2C_SR1_RXNE))
		;
#ifdef I2C_DEBUG_USING_LED
	red_Off();
#endif
	*data = I2C1->DR;
}

void i2c_Read(uint8_t *data)
{
//	i2c_Begin(address);
//	I2C1->CR1 |= I2C_CR1_STOP;
#ifdef I2C_DEBUG_USING_LED
	red_On();
#endif
	while (!(I2C1->SR1 & I2C_SR1_RXNE))
		;
#ifdef I2C_DEBUG_USING_LED
	red_Off();
#endif
	*data = I2C1->DR;
}

void i2c_Read_end(uint8_t *data)
{
//	i2c_Begin(address);
	I2C1->CR1 |= I2C_CR1_STOP;
#ifdef I2C_DEBUG_USING_LED
	red_On();
#endif
	while (!(I2C1->SR1 & I2C_SR1_RXNE))
		;
#ifdef I2C_DEBUG_USING_LED
	red_Off();
#endif
	*data = I2C1->DR;
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

