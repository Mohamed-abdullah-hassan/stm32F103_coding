/*
 * system.h
 *
 *  Created on: Nov 25, 2023
 *      Author: engmo
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "stm32f103xb.h"

typedef enum
{
    /* GENERAL POURPOSE IO*/
    Peripherals_GpioA = 0b00010001,     // 0x11
    Peripherals_GpioB = 0b00010010,     // 0x12
    Peripherals_GpioC = 0b00010011,     // 0x13
    Peripherals_GpioD = 0b00010100,     // 0x14
    Peripherals_GpioE = 0b00010101,     // 0x15
//	peripheral_GPIO_F	=	0b00010110, // 0x16
//	peripheral_GPIO_G	=	0b00010111, // 0x17
//	peripheral_GPIO_H	=	0b00011000, // 0x18

    Peripherals_Afio = 0b00011111,     // 0x1F

    /*Timers */
    Peripherals_Timer1 = 0b00100000,     // 0x20
    Peripherals_Timer2 = 0b00100001,     // 0x21
    Peripherals_Timer3 = 0b00100010,     // 0x22
    Peripherals_Timer4 = 0b00100011,     // 0x23
//	Peripherals_Timer5	=	0b00100100, // 0x24
//	Peripherals_Timer6	=	0b00100101, // 0x25
//	Peripherals_Timer7	=	0b00100110, // 0x26
//	Peripherals_Timer8	=	0b00100111, // 0x27
//	Peripherals_Timer9	=	0b00101000, // 0x28
//	Peripherals_Timer10	=	0b00101001, // 0x29
//	Peripherals_Timer11	=	0b00101010, // 0x2A
//	Peripherals_Timer12	=	0b00101011, // 0x2B
//	Peripherals_Timer13	=	0b00101100, // 0x2C
//	Peripherals_Timer14	=	0b00101101, // 0x2D
//	Peripherals_Timer15	=	0b00101110, // 0x2E
//	Peripherals_Timer16	=	0b00101111, // 0x2F

    /*ADC and DAC*/
    Peripherals_Adc1 = 0b00110001,     // 0x31
    Peripherals_Adc2 = 0b00110010,     // 0x32
//	Peripherals_Adc3	=	0b00110011, // 0x33
    Peripherals_Dac1 = 0b00110100,     // 0x34
    /*Communication Peripherals*/
    /*SPI*/
    Peripherals_Spi1 = 0b01001001,     // 0x49
    Peripherals_Spi2 = 0b01001010,     // 0x4A
    /*USART*/
    Peripherals_Usart1 = 0b01010001,     // 0x51
    Peripherals_Usart2 = 0b01010010,     // 0x52
    Peripherals_Usart3 = 0b01010011,     // 0x53
    /*I2C*/
    Peripherals_I2c1 = 0b01011001,     // 0x59
    Peripherals_I2c2 = 0b01011010,     // 0x5A
    /*USB*/
    Peripherals_Usb1 = 0b01100001,     // 0x61
    /*CAN*/
    Peripherals_Can1 = 0b01100101,     // 0x65

    /*System Bus*/
    Peripherals_Ahb = 0b10000001,     // 0x81
    Peripherals_Apb1 = 0b10000010,     // 0x82
    Peripherals_Apb2 = 0b10000011,     // 0x83
    Peripherals_Systick = 0b10001000,     // 0x88

} system_Peripherals_e;



typedef enum
{
    BusClock_HCLK       =   0,
    BusClock_PCLK1,
    BusClock_PCLK2,
    BusClock_SYSCLK,
}system_BusClock_e;

typedef enum
{
    Response_Error = 0x00,
    Response_OK = 0x01,
    Response_NotImplemented = 0x02,
    Response_Invalid = 0x11,

    //GPIO messages
    Response_GpioInvalidPort,
    Response_GpioInvalidPinMode,

} system_Response_e;

typedef struct
{
        system_BusClock_e   bus_name;
        uint32_t            frequency;

}system_Clock_t;

system_Response_e system_PeripheralEnable(system_Peripherals_e peripheral );
system_Response_e system_PeripheralDisable(system_Peripherals_e peripheral );

void system_Calculate_Sysclk(void);
void system_getBusFrequency(system_Clock_t* bus);

#endif /* SYSTEM_H_ */
