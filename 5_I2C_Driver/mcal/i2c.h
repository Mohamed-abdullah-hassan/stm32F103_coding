/*
 * i2c.h
 *
 *  Created on: Dec 10, 2023
 *      Author: engmo
 */

#ifndef MCAL_I2C_H_
#define MCAL_I2C_H_
#include "system.h"
#include "stdint.h"
#include <stdbool.h>

//typedef enum
//{
//    i2cSpeed_Sm = 0,
//    i2cSpeed_Fm,
//} i2c_I2cSpeed;

typedef enum
{
    i2c_Option_enMasterMode     = 0b00000001,
    i2c_Option_enClockStrech    = 0b00000010,
    i2c_Option_enTxInterrupt    = 0b00000100,
    i2c_Option_enRxInterrupt    = 0b00001000,
    i2c_Option_enErrInterupt    = 0b00010000,
    i2c_Option_en2ndDualAddress = 0b00100000,
    i2c_Option_en10BitAddress   = 0b01000000,
    i2c_Option_enFastMode       = 0b10000000,
} i2c_Option_e;


typedef enum
{
    i2cError_NoError = 0,
//    i2cError_StartGenerated,
//    i2cError_StopGenerated,
//    i2cError_AddressSent,
    i2cError_BusBusy,
    i2cError_BusError,
    i2cError_TimeOut,
    i2cError_AcknowledgeFailed,
    i2cError_ArbitrationLost,
    i2cError_RxNotEmpty,
    i2cError_OverRun,
} i2c_I2cErrorCode_e;

typedef enum
{
    i2cStatus_NotInit = 0,
    i2cStatus_Ok,
    i2cStatus_StartGenerated,
    i2cStatus_StopGenerated,
    i2cStatus_AddressSent,
    i2cStatus_MasterWrite,
    i2cStatus_MasterRead,
    i2cStatus_SlaveWrite,
    i2cStatus_SlaveRead,
} i2c_I2cStatus_e;

typedef enum
{
    i2c_Response_Ok,
    i2c_Response_Error,
    i2c_Response_AFNotImplemented,
}i2c_Response_e;

typedef struct
{
    i2c_I2cStatus_e     descreptor_Status;
    i2c_Option_e        descreptor_Option;
    i2c_I2cErrorCode_e  descreptor_ErrorCode;
    I2C_TypeDef         descriptor_Port;
} i2c_Descriptor_t;

i2c_Response_e i2c_Init(i2c_Descriptor_t* i2c, system_Peripherals_e peripheral, bool pinAlernateFunction);

#endif /* MCAL_I2C_H_ */
