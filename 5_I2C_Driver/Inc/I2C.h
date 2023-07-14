#ifndef __I2C___H
#define __I2C___H
#include "stdint.h"

void i2c1_Init();
void i2c1_Write_Begin(uint8_t s_address, uint8_t data);
void i2c1_write_buff(uint8_t *data, uint16_t count);
void i2c1_write(uint8_t data);
void i2c1_End();

#endif
