/*
* i2c.c
 *
 *  Created on: Dec 10, 2023
 *      Author: engmo
 */

#include "i2c.h"
#include <stddef.h>
#include <stdbool.h>
#include "gpio.h"


#define I2C_GET_APB_BUS_CLOCK_MHz           (8)
#define I2C_CALC_CCR_VALUE(BUS_CLOCK, FM)   (((FM != 1)?40:0xff))
#define I2C_CALC_TRISE                      (9)

#define I2C_OAR1_BIT_14_KEEP_SET    (1 <<  14)

#if 0
#define I2C_CR1_SWRST       (1 << 15)
#define I2C_CR1_ALERT       (1 << 13)
#define I2C_CR1_PEC         (1 << 12)
#define I2C_CR1_POS         (1 << 11)
#define I2C_CR1_ACK         (1 << 10)
#define I2C_CR1_STOP        (1 <<  9)
#define I2C_CR1_START       (1 <<  8)
#define I2C_CR1_NO_STRETCH  (1 <<  7)
#define I2C_CR1_ENGC        (1 <<  6)
#define I2C_CR1_ENPEC       (1 <<  5)
#define I2C_CR1_ENARP       (1 <<  4)
#define I2C_CR1_SMB_TYPE    (1 <<  3)
#define I2C_CR1_SMBUS       (1 <<  1)
#define I2C_CR1_PE          (1 <<  0)

#define I2C_CR2_LAST        (1 << 12)
#define I2C_CR2_DMAEN       (1 << 11)
#define I2C_CR2_ITBUFEN     (1 << 10)
#define I2C_CR2_ITEVTEN     (1 <<  9)
#define I2C_CR2_ITERREN     (1 <<  8)
#define I2C_CR2_FREQ_MASK   (0b00011111)

#define I2C_OAR1_ADD_MODE           (1 <<  15)
#define I2C_OAR1_BIT_14_KEEP_SET    (1 <<  14)
#define I2C_OAR1_ADD_9_8_MASK       (0b000000110000000)
#define I2C_OAR1_ADD_9_8_SHIFT      (8)
#define I2C_OAR1_ADD7BIT_MASK       (0b11111110)
#define I2C_OAR1_ADD7BIT_SHIFT      (0b01111110)
#define I2C_OAR1_ADD0               (1 <<  0)

#define I2C_OAR2_ADD2_MASK          (0b11111110)
#define I2C_OAR2_ENDUAL             (1 <<  0)

#define I2C_SR1_SMB_ALERT           (1 << 15)
#define I2C_SR1_TIME_OUT            (1 << 14)
#define I2C_SR1_PEC_ERR             (1 << 12)
#define I2C_SR1_OVR                 (1 << 11)
#define I2C_SR1_AF                  (1 << 10)
#define I2C_SR1_ARLO                (1 <<  9)
#define I2C_SR1_BERR                (1 <<  8)
#define I2C_SR1_TXE                 (1 <<  7)
#define I2C_SR1_RXNE                (1 <<  6)
#define I2C_SR1_STOPF               (1 <<  4)
#define I2C_SR1_ADD10               (1 <<  3)
#define I2C_SR1_BTF                 (1 <<  2)
#define I2C_SR1_ADR                 (1 <<  1)
#define I2C_SR1_SB                  (1 <<  0)

#define I2C_SR2_PEC_MASK            (0b1111111100000000)
#define I2C_SR2_PEC_SHIFT           (1 <<  8)
#define I2C_SR2_DUALF               (1 <<  7)
#define I2C_SR2_SMB_HOST            (1 <<  6)
#define I2C_SR2_SMBDE_FAULT         (1 <<  5)
#define I2C_SR2_GEN_CALL            (1 <<  4)
#define I2C_SR2_TRA                 (1 <<  2)
#define I2C_SR2_BUSY                (1 <<  1)
#define I2C_SR2_MSL                 (1 <<  0)

#define I2C_CCR_FS                  (1 << 15)
#define I2C_CCR_DUTY                (1 << 14)
#define I2C_CCR_CCR_MASK            (0b0000111111111111)
#define I2C_CCR_CCR_SHIFT           (0)

#define I2C_TRISE_MASK              (0b00111111)
#define I2C_TRISE_SHIFT             (0)

#endif

I2C_TypeDef* i2c_getPeripheral(system_Peripherals_e peripheral)
{
    I2C_TypeDef* port = NULL;
    switch (peripheral)
    {
        case Peripherals_I2c1:
            port = I2C1;
            break;
        case Peripherals_I2c2:
            port = I2C2;
            break;
        default:
            break;
    }

    return port;
}

i2c_Response_e i2c_getI2cPin(system_Peripherals_e peripheral, gpio_Pin_t* scl,gpio_Pin_t* sda, bool alternateFunction)
{
    i2c_Response_e res = i2c_Response_Error;

    switch (peripheral)
    {
        case Peripherals_I2c1:
            scl->pin_Port = Peripherals_GpioB;
            sda->pin_Port = Peripherals_GpioB;
//            scl->pin_Number = ((alternateFunction)? 8 : 6);
//            sda->pin_Number = ((alternateFunction)? 9 : 7);
            if(true == alternateFunction)
            {
                res = i2c_Response_AFNotImplemented;
            }
            else
            {
                scl->pin_Number   = 6;
                sda->pin_Number   = 7;

                res = i2c_Response_Ok;
            }
            break;
        case Peripherals_I2c2:
            if(false == alternateFunction)
            {
                scl->pin_Port   = Peripherals_GpioB;
                sda->pin_Port   = Peripherals_GpioB;
                scl->pin_Number = 10;
                sda->pin_Number = 11;
                res             = i2c_Response_Ok;
            }
            else
            {
                res = i2c_Response_Error;
            }
            break;
        default:
            break;
    }

    return res;
}

i2c_Response_e i2c_Init(i2c_Descriptor_t* i2c, system_Peripherals_e peripheral, bool pinAlernateFunction)
{
    I2C_TypeDef*        i2c_Port = NULL;
    i2c_Response_e      response = i2c_Response_Error;
    system_Response_e   err;
    gpio_Pin_t  scl;
    gpio_Pin_t  sda;

    err = system_PeripheralEnable(peripheral);
    if(Response_OK == err)
    {
        i2c_Port = i2c_getPeripheral(peripheral);
    }
    if(i2c_Port != NULL)
    {
        response = i2c_getI2cPin(peripheral, &scl, &sda, pinAlernateFunction);
    }
    if(i2c_Response_Ok == response)
    {
        i2c_Port->CR1 |=  I2C_CR1_SWRST;
        i2c_Port->CR1 &= (~(I2C_CR1_SWRST));
        i2c_Port->CR2  = ((I2C_GET_APB_BUS_CLOCK_MHz)&(0X3F));
        if ((i2c->descreptor_Option) & i2c_Option_enFastMode)
        {
            i2c_Port->CCR |= (I2C_CCR_FS
                    | (I2C_CALC_CCR_VALUE(I2C_GET_APB_BUS_CLOCK_MHz, 1)));
        }
        else
        {
            i2c_Port->CCR |= ( I2C_CALC_CCR_VALUE(I2C_GET_APB_BUS_CLOCK_MHz,0) );
        }

        i2c_Port->OAR1  = I2C_OAR1_BIT_14_KEEP_SET;
        i2c_Port->TRISE = I2C_CALC_TRISE;
        i2c_Port->CR1   |= I2C_CR1_ACK;

        scl.pin_Config = pinConfig_AltereFuncOutputOpenDrain;
        sda.pin_Config = pinConfig_AltereFuncOutputOpenDrain;
        scl.pin_Mode   = pinMode_Output50MHz;
        sda.pin_Mode   = pinMode_Output50MHz;
        gpio_PinInit(&scl);
        gpio_PinInit(&sda);

        /*Enable the I2C peripheral */
        i2c_Port->CR1 |= I2C_CR1_PE;

        response = i2c_Response_Ok;
    }
    return response;
}

#if 0
/*Enable clock to i2c1 module on APB1 Bus*/
/*Initiate I2C peripheral reset*/

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
/*Enable the I2C peripheral */
I2C1->CR1 |= I2C_CR1_PE;
#endif
