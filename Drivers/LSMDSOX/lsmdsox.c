#include "lsmdsox.h"


/**
  * @brief  
  * @param  dev Pointer to LSMDOX structure that contains configuration information
  * @param  i2cHandle Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
  * @retval errNum Number of errors
  */
uint8_t	LSMDSOX_Initialize(LSMDSOX *dev, I2C_HandleTypeDef *i2cHandle){

	/* Set struct parameters */
	dev->i2cHandle 			= i2cHandle;

	dev->accelerometer_x	= 0;
	dev->accelerometer_y	= 0;
	dev->accelerometer_z	= 0;

	dev->gyroscope_x		= 0;
	dev->gyroscope_y		= 0;
	dev->gyroscope_z		= 0;

	dev->temp_c				= 0;

	/* Store number of errors (to be returned at the end of the function) */
	uint8_t	errNum = 0;
	HAL_StatusTypeDef status;

	/*
	 * Initialization (pg. 14)
	 */
	uint8_t regData;

	status = LSMDSOX_ReadRegister(dev, WHO_AM_I, &regData);
	errNum += (status != HAL_OK);

	if( regData != LSMDSOX_DEVICE_ID){

		return 255;
	}

	/* Powers on accelerometer & selects full-scale 8g (pg. 56)*/
	regData = 0x4C
	status = LSMDSOX_writeRegister(dev, CTRL1_XL, &regData);
	errNum += (status != HAL_OK);

	/* Powers on gyroscope & selects full-scale 250dps (pg. 57)*/
	regData = 0x40
	status = LSMDSOX_writeRegister(dev, CTRL2_G, &regData);
	errNum += (status != HAL_OK);


	
	return errNum;

}


/*
 * DATA AQUISITION 
 */

/**
  * @brief  
  * @param  dev Pointer to LSMDOX structure that contains configuration information
  * @retval HAL status
 **/
/*	DATASHEET PAGE  72*/
HAL_StatusTypeDef LSMDSOX_ReadAccelerometer(LSMDSOX *dev){

	uint8_t regData[3];
	int16_t rawData;
	HAL_StatusTypeDef status;



	/*
	 * Read raw values from Accelerometer Output Registers (16 bits two's complement)
	 */
	status = LSMDSOX_ReadRegisters(dev, OUTX_L_A, &regData, 6);


	rawData = ((int16_t)regData[1] << 8) | regData[0];
	dev->accelerometer_x = ((float_t)rawData) * 0.244f;

	rawData = ((int16_t)regData[3] << 8) | regData[2];
	dev->accelerometer_y = ((float_t)rawData) * 0.244f;

	rawData = ((int16_t)regData[5] << 8) | regData[4];
	dev->accelerometer_z = ((float_t)rawData) * 0.244;




	return status;


}

/**
  * @brief  
  * @param  dev Pointer to LSMDOX structure that contains configuration information
  * @retval HAL status
 **/
HAL_StatusTypeDef LSMDSOX_ReadGyroscope(LSMDSOX *dev){

	uint8_t regData[6];
	int16_t rawData;
	HAL_StatusTypeDef status;




	/*
	 * Read raw values from Gyroscope Output Registers (16 bits two's complement)
	 */
	status = LSMDSOX_ReadRegisters(dev, OUTX_L_G, &regData, 6);


	rawData = ((int16_t)regData[1] << 8) | regData[0];
	dev->gyroscope_x = ((float_t)rawData) * 8.750;

	rawData = ((int16_t)regData[3] << 8) | regData[2];
	dev->gyroscope_y = ((float_t)rawData) * 8.750;

	rawData = ((int16_t)regData[5] << 8) | regData[4];
	dev->gyroscope_z = ((float_t)rawData) * 8.750;



	return status;


}


/**
  * @brief  
  * @param  dev Pointer to LSMDOX structure that contains configuration information
  * @retval HAL status
 **/
/*	DATASHEET PAGE  70 */
HAL_StatusTypeDef LSMDSOX_ReadTemperature(LSMDSOX *dev){

	uint8_t regData[2];
	int16_t rawData;
	HAL_StatusTypeDef status;

	/*
	 * Read raw values from Temperature registers (16 bits two's complement)
	 */
	status = LSMDSOX_ReadRegisters(dev, OUT_TEMP_L, &regData, 2);


	rawData = ((int16_t)regData[0] << 8) | (regData[1]);

	dev->temp_c = ((float_t)rawData / 256.0) + 25.0;



	return status;

}


/*
 * LOW-LEVEL FUNCTIONS 
 */

/**
  * @brief  Read an amount of data in blocking mode from a specific memory address
  * @param  dev Pointer to LSMDOX structure that contains i2cHaandle
  * @param  reg Internal memory address
  * @param  data Pointer to data buffer
  * @retval HAL status
  */
HAL_StatusTypeDef LSMDSOX_ReadRegister(LSMDSOX *dev, uint8_t reg, uint8_t *data){

	return HAL_I2C_Mem_Read( dev->i2cHandle, LSMDSOX_I2C_ADDR, reg, I2C_MEMADD_SIZE_8BIT, data, 1, HAL_MAX_DELAY);

}

/**
  * @brief  Read a specific amount of data in blocking mode from a specific memory address
  * @param  dev Pointer to LSMDOX structure that contains i2cHaandle
  * @param  reg Internal memory address
  * @param  data Pointer to data buffer
  * @param  length Amount of data to be read
  * @retval HAL status
  */
HAL_StatusTypeDef LSMDSOX_ReadRegisters(LSMDSOX *dev, uint8_t reg, uint8_t *data, uint8_t length){

	return HAL_I2C_Mem_Read( dev->i2cHandle, LSMDSOX_I2C_ADDR, reg, I2C_MEMADD_SIZE_8BIT, data, length, HAL_MAX_DELAY);

}

/**
  * @brief  Write an amount of data in blocking mode from a specific memory address
  * @param  dev Pointer to LSMDOX structure that contains i2cHaandle
  * @param  reg Internal memory address
  * @param  data Pointer to data buffer
  * @retval HAL status
  */
HAL_StatusTypeDef LSMDSOX_WriteRegister(LSMDSOX *dev, uint8_t reg, uint8_t *data){

	return HAL_I2C_Mem_Write( dev->i2cHandle, LSMDSOX_I2C_ADDR, reg, I2C_MEMADD_SIZE_8BIT, data, 1, HAL_MAX_DELAY);

}
