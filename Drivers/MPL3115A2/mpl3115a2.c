#include "mpl3115a2.h"

uint8_t	MPL3115A2_Initialize(MPL3115A2 *dev, I2C_HandleTypeDef *i2cHandle){

	/* Set struct parameters */
	dev->i2cHandle 		= i2cHandle;

	dev->pressure_int	= 0;
	dev->pressure_dec	= 0;

	dev->altitude_int	= 0;
	dev->altitude_dec	= 0;


	dev->temp_c_int		= 0;
	dev->temp_c_dec		= 0;

	/* Store number of errors (to be returned at the end of the function) */
	uint8_t	errNum = 0;
	HAL_StatusTypeDef status;

	/*
	 * Initialization (pg. 14)
	 */
	uint8_t regData;
	uint8_t STA = 0;

	status = MPL3115A2_ReadRegister(dev, WHO_AM_I, &regData);
	errNum += (status != HAL_OK);

	if( regData != MPL3115A2_DEVICE_ID){

		return 255;
	}

	do{

		/* Enable Data Flags in PT_DATA_CFG */
		regData = 0x07;
		status = MPL3115A2_WriteRegister(dev, PT_DATA_CFG, &regData);
		errNum += (status != HAL_OK);


		/* Read STATUS Register */
		MPL3115A2_ReadRegister(dev, DR_STATUS, &STA);
		errNum += (status != HAL_OK);




	}while( (STA & 0x08) != 0b00001000 );


	return errNum;

}


/*
 * DATA AQUISITION
 */
/*	DATASHEET PAGE  */
HAL_StatusTypeDef MPL3115A2_ReadAltitude(MPL3115A2 *dev){

	uint8_t regData[3];
	uint8_t sensor_mode = ALTITUDE_MODE;
	HAL_StatusTypeDef status;


	/* Set Active Altitude */
	status = MPL3115A2_WriteRegister(dev, CTRL_REG1, &sensor_mode);


	/*
	 * Read raw values from Altimeter registers (20 bits two's complement)
	 */
	status = MPL3115A2_ReadRegisters(dev, OUT_P_MSB, &regData, 3);



	dev->altitude_int = (((int16_t) regData[0]  << 8) |  regData[1] );

    dev->altitude_dec = regData[2] >> 4;




	return status;


}
HAL_StatusTypeDef MPL3115A2_ReadPressure(MPL3115A2 *dev){

	uint8_t regData[3];
	int8_t off_H = 0;
	uint8_t sensor_mode = PRESSURE_MODE;
	HAL_StatusTypeDef status;


	/* Set Active Pressure */
	status = MPL3115A2_WriteRegister(dev, CTRL_REG1, &sensor_mode);


	/*
	 * Read raw values from Altimeter registers (20 bits two's complement)
	 */
	status = MPL3115A2_ReadRegisters(dev, OUT_P_MSB, &regData, 3);



	dev->pressure_int = ((int32_t)(((int16_t) regData[0]  << 8) |  regData[1] ) << 2) | (regData[2] >> 6)  ;

    dev->pressure_dec = (regData[2] & 0b00110000) >> 4;

    status = MPL3115A2_ReadRegister(dev, OFF_H, &off_H);





	return status;


}


/*	DATASHEET PAGE  */
HAL_StatusTypeDef MPL3115A2_ReadTemperature(MPL3115A2 *dev){

	uint8_t regData[2];
	HAL_StatusTypeDef status;

	/*
	 * Read raw values from Temperature registers (12 bits two's complement)
	 */
	status = MPL3115A2_ReadRegisters(dev, OUT_T_MSB, &regData, 2);


	dev->temp_c_int = regData[0];
	dev->temp_c_dec = regData[1] >> 4;


	return status;

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
