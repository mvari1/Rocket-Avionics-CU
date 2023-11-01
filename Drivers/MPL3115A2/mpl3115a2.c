#include "mpl3115a2.h"

uint8_t	MPL3115A2_Initialize(MPL3115A2 *dev, I2C_HandleTypeDef *i2cHandle){

}


/*
 * DATA AQUISITION
 */
/*	DATASHEET PAGE  */
HAL_StatusTypeDef MPL3115A2_ReadMagnetometer(MPL3115A2 *dev){
}


/*	DATASHEET PAGE  */
HAL_StatusTypeDef MPL3115A2_ReadTemperature(MPL3115A2 *dev){

}


/*
 * LOW-LEVEL FUNCTIONS
 */
HAL_StatusTypeDef MPL3115A2_ReadRegister(MPL3115A2 *dev, uint8_t reg, uint8_t *data){

	return HAL_I2C_Mem_Read( dev->i2cHandle, MPL3115A2_I2C_ADDR, reg, I2C_MEMADD_SIZE_8BIT, data, 1, HAL_MAX_DELAY);

}
HAL_StatusTypeDef MPL3115A2_ReadRegisters(MPL3115A2 *dev, uint8_t reg, uint8_t *data, uint8_t length){

	return HAL_I2C_Mem_Read( dev->i2cHandle, MPL3115A2_I2C_ADDR, reg, I2C_MEMADD_SIZE_8BIT, data, length, HAL_MAX_DELAY);

}

HAL_StatusTypeDef MPL3115A2_WriteRegister(MPL3115A2 *dev, uint8_t reg, uint8_t *data){

	return HAL_I2C_Mem_Write( dev->i2cHandle, MPL3115A2_I2C_ADDR, reg, I2C_MEMADD_SIZE_8BIT, data, 1, HAL_MAX_DELAY);

}
