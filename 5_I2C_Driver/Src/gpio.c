/*
 * gpio.c
 *
 *  Created on: Nov 26, 2023
 *      Author: engmo
 */
#include "gpio.h"
#include <stddef.h>

#define GPIO_Port	(GPIO_TypeDef *)

GPIO_TypeDef* gpio_GetPort(system_Peripherals_e gpio_port )
{
    GPIO_TypeDef * port = NULL;
    if ((gpio_port >= Peripherals_GpioA) & (gpio_port <= Peripherals_GpioE))
    {
        switch (gpio_port)
        {
            case Peripherals_GpioA:
                port = GPIOA;
                break;
            case Peripherals_GpioB:
                port = GPIOB;
                break;
            case Peripherals_GpioC:
                port = GPIOC;
                break;
            case Peripherals_GpioD:
                port = GPIOD;
                break;
            case Peripherals_GpioE:
                port = GPIOE;
                break;
            default:
                break;
        }
    }
    return port;
}

system_Response_e gpio_PinInit(gpio_Pin_t *pin )
{
    //TODO: check if pin number is SWD debug pin and return an error message to use a specific function to alter SWD debug pin
    GPIO_TypeDef * gpio = gpio_GetPort(pin->pin_Port);
    if (gpio == NULL) return Response_GpioInvalidPort;
    system_Response_e en = system_PeripheralEnable(pin->pin_Port);
    if (en != Response_OK) return en;

    uint8_t configuration = ((pin->pin_Mode & 0b0011) | ((pin->pin_Config & 0b0011) << 2));
    if (pin->pin_Mode == pinMode_Input)
        if ((configuration & 0b11) == 0b11) return Response_GpioInvalidPinMode;
    // Port Configuration low register for pin from 0 to 7
    if (pin->pin_Number < 8)
    {
        gpio->CRL &= ~(0b1111 << (4 * pin->pin_Number));
        gpio->CRL |= (configuration << (4 * pin->pin_Number));
    }

    // Port Configuration high register for pin from 8 to 15
    else if ((pin->pin_Number >= 8) & (pin->pin_Number <= 15))
    {
        gpio->CRH &= ~(0b1111 << (4 * (pin->pin_Number - 8)));
        gpio->CRH |= (configuration << (4 * (pin->pin_Number - 8)));
    }
    return Response_OK;
}

system_Response_e gpio_SetPinHigh(gpio_Pin_t *pin )
{
    GPIO_TypeDef * gpio = gpio_GetPort(pin->pin_Port);
    if (gpio == NULL) return Response_GpioInvalidPort;
    // TODO check if GPIO port is enabled and pin is configured
    gpio->BSRR |= (1 << pin->pin_Number);
    return Response_OK;
}

system_Response_e gpio_SetPinLow(gpio_Pin_t *pin )
{
    GPIO_TypeDef * gpio = gpio_GetPort(pin->pin_Port);
    if (gpio == NULL) return Response_GpioInvalidPort;
    // TODO check if GPIO port is enabled and pin is configured
    gpio->BRR |= (1 << pin->pin_Number);
    return Response_OK;
}
