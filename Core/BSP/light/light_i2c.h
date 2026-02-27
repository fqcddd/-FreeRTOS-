//
// Created by keysk on 2025/11/20.
//

#ifndef SMARTFARM_LIGHT_I2C_H
#define SMARTFARM_LIGHT_I2C_H
#include "main.h"

uint8_t I2C_WriteData(uint8_t SlaveAddress, uint8_t *Data, uint8_t len);
uint8_t I2C_ReadDate(uint8_t SlaveAddress, uint8_t *Data, uint8_t len);

#endif //SMARTFARM_LIGHT_I2C_H