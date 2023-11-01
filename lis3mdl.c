#include "lis3mdl.h"

uint8_t	LIS3MDL_Initialize(LIS3MDL *dev, I2C_HandleTypeDef *i2cHandle){

	/* Set struct parameters */
	dev->i2cHandle 		= i2cHandle;

	dev->mag_guass[0]		= 0.0f;
	dev->mag_guass[1]		= 0.0f;
	dev->mag_guass[2]		= 0.0f;


	dev->temp_c				= 0.0f;

	/* Store number of errors (to be returned at the end of the function) */
	uint8_t	errNum = 0;
	HAL_StatusTypeDef status;

	/*
	 * Check device ID is correct (pg. 24)
	 */
	uint8_t regData;

	status = LIS3MDL_ReadRegister( dev, WHO_AM_I, &regData);
	errNum += (status != HAL_OK);

	if( regData != LIS3MDL_DEVICE_ID){

		return 255;
	}

	/* 
	 * Enable the temperature sensor (p. 24) 
	 */
	regData = 0x90;

	status = LIS3MDL_WriteRegister(dev, CTRL_REG1, &regData);
	errNum += (status != HAL_OK );


	/* Checking it wrote to the register */
	status = LIS3MDL_ReadRegister(dev, CTRL_REG1, &regData);
	errNum += (0x90 != regData );

	/* 
	 * Put sensor in to measurement mode (p. 26) 
	 */
	regData = 0x00;

	status = LIS3MDL_WriteRegister(dev, CTRL_REG3, &regData);
	errNum += (status != HAL_OK );

	/* Checking it wrote to the register */
	status = LIS3MDL_ReadRegister(dev, CTRL_REG3, &regData);
	errNum += (0x00 != regData );

	/* Return number of erros (0 if successful initialization) */
	return errNum;

}

/*
 * DATA AQUISITION
 */
/* Internal temperature sensor that is suitable for delta temperature measurement (pg. 20 of AN4602) */
HAL_StatusTypeDef LIS3MDL_ReadTemperature_DT(LIS3MDL *dev){
	/*	DATASHEET PAGE 9 */

	/*
	 * Read raw values from temperature registers (16 bits two's complement)
	 */
	uint8_t regData[2];


	HAL_StatusTypeDef status = LIS3MDL_ReadRegisters(dev, TEMP_OUT_L, regData[0], 1);
	status = LIS3MDL_ReadRegisters(dev, TEMP_OUT_H, regData[1], 1);

	/*
	 * Combine register values to give raw temperature reading (16 bits two's complement)
	 */
	int16_t tempRaw = (((int16_t)regData[1] << 8) | (regData[0])); /* X-axis */


	/* Convert deg C (offset @ 25 degC, slope = 8.0 LSB/degC) */
	dev->temp_c = ((float)tempRaw / 256.0) + 25.0;

	return status;

}


HAL_StatusTypeDef LIS3MDL_ReadMagnetometer(LIS3MDL *dev){
	/*	DATASHEET PAGE 8 */

	/*
	 * Read raw values from magnetometer registers (16 bits two's complement)
	 */
	uint8_t regData[6];

	HAL_StatusTypeDef status = LIS3MDL_ReadRegisters(dev, OUT_X_L, regData, 6);

	/*
	 * Combine register values  to give raw magnetometer reading (16 bits two's complement)
	 */
	int16_t magRawSigned[3];

	magRawSigned[0] = ((int16_t) regData[1]<<8 |  regData[0]); /* X-axis */
	magRawSigned[1] = ((int16_t) regData[3]<<8 |  regData[2]); /* Y-axis */
	magRawSigned[2] = ((int16_t) regData[5]<<8 |  regData[4]); /* Z-axis */
	

	/* Converting to gauss (given range setting of +- 4 gauss) */
	dev->mag_guass[0] = (float)magRawSigned[0] * 0.0001461560947f;
	dev->mag_guass[1] = (float)magRawSigned[1] * 0.0001461560947f;
	dev->mag_guass[2] = (float)magRawSigned[2] * 0.0001461560947f;

	return status;

}

/*
 * LOW-LEVEL FUNCTIONS
 */
HAL_StatusTypeDef LIS3MDL_ReadRegister(LIS3MDL *dev, uint8_t reg, uint8_t *data){

	return HAL_I2C_Mem_Read( dev->i2cHandle, LIS3MDL_I2C_ADDR, reg, I2C_MEMADD_SIZE_8BIT, data, 1, HAL_MAX_DELAY);

}
HAL_StatusTypeDef LIS3MDL_ReadRegisters(LIS3MDL *dev, uint8_t reg, uint8_t *data, uint8_t length){

	return HAL_I2C_Mem_Read( dev->i2cHandle, LIS3MDL_I2C_ADDR, reg, I2C_MEMADD_SIZE_8BIT, data, length, HAL_MAX_DELAY);

}

HAL_StatusTypeDef LIS3MDL_WriteRegister(LIS3MDL *dev, uint8_t reg, uint8_t *data){

	return HAL_I2C_Mem_Write( dev->i2cHandle, LIS3MDL_I2C_ADDR, reg, I2C_MEMADD_SIZE_8BIT, data, 1, HAL_MAX_DELAY);

}


