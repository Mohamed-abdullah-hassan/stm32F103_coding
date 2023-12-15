/*
 * gpio.h
 *
 *  Created on: Nov 26, 2023
 *      Author: engmo
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "stm32f103xb.h"
#include "system.h"

typedef enum
{
    pinMode_Input           = 0b00,
    pinMode_Output2MHz     = 0b10,
    pinMode_Output10MHz    = 0b01,
    pinMode_Output50MHz    = 0b11,
} gpio_PinMode_e;

typedef enum
{
    // Input mode pin configuration
    pinConfig_InputAnalog       = 1,
    pinConfig_InputFloat        = 2,
    pinConfig_InputPullUPDown   = 3,

    // General Output pin configuration
    pinConfig_OutputPushPull    = 4,
    pinConfig_OutputOpenDrain   = 5,

    // Alternate function pin configuration
    pinConfig_AlterFuncOutputPushPull   = 6,
    pinConfig_AltereFuncOutputOpenDrain = 7,
} gpio_PinConfig_e;

typedef struct
{
    system_Peripherals_e pin_Port;
    uint8_t              pin_Number;
    gpio_PinMode_e       pin_Mode;
    gpio_PinConfig_e     pin_Config;
} gpio_Pin_t;

system_Response_e gpio_PinInit     (gpio_Pin_t *pin );
system_Response_e gpio_SetPinHigh  (gpio_Pin_t *pin );
system_Response_e gpio_SetPinLow   (gpio_Pin_t *pin );

#endif /* GPIO_H_ */
