/*
 *
 * 	MPL3115A2 Altimetry I2C Driver 
 *
 *	Author:	Mason Vari
 *	Created: 31 October 2023
 *
 */


/* Prevents recursive inclusion -------------------------------------*/
#ifndef MPL3115A2_I2C_DRIVER_H
#define MPL3115A2_I2C_DRIVER_H


/* Includes ---------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/*
 * DEFINES (pg. 19 - 20)
 */
#define MPL3115A2_I2C_ADDR			(0xC0 )  /*Read (0xC1), Write (0xC0) */
#define WHO_AM_I				0x0C	
#define MPL3115A2_DEVICE_ID			0xC4
#define PT_DATA_CFG				0x13
#define SYSMOD					0x11

/*
 * CONTROL REGISTERS 
 */
#define CTRL_REG1				0x26
#define PRESSURE_MODE		 		0x39
#define ALTITUDE_MODE				0xB9
#define CTRL_REG2				0x27
#define CTRL_REG3				0x28
#define CTRL_REG4				0x29
#define CTRL_REG5				0x2A


/*
 * STATUS REGISTER 
 */
#define STATUS					0x00
#define DR_STATUS				0x06  /*Check Data Sheet Based on Configuration*/
#define F_STATUS				0x0D
#define OFF_H 					0x2D  /* Off Set for Altitude Calculations */

/*
 * OUTPUT REGISTERS 
 */
#define OUT_P_MSB				0x01
#define OUT_P_CSB				0x02
#define OUT_P_LSB				0x03
#define OUT_T_MSB				0x04
#define OUT_T_LSB				0x05
#define OUT_P_DELTA_MSB				0x07
#define OUT_P_DELTA_CSB				0x08
#define OUT_P_DELTA_LSB				0x09
#define OUT_T_DELTA_MSB				0x0A
#define OUT_T_DELTA_LSB				0x0B



/*
 * INTERRUPT REGISTERS
 */
#define INT_SOURCE				0x12


/*
 * SENSOR STRUCT
 */

typedef struct {

	/* I2C handle */
	I2C_HandleTypeDef *i2cHandle;

	/* Pressure data in kPa */
	uint32_t pressure_int;
	uint8_t pressure_dec;

	/* Pressure data in m */
	int16_t altitude_int;
	uint8_t altitude_dec;


	/* Temperature data in celsius */
	int16_t temp_c_int;
	uint8_t temp_c_dec;


} MPL3115A2;

/*
 * INITIALIZATION
 */
uint8_t	MPL3115A2_Initialize(MPL3115A2 *dev, I2C_HandleTypeDef *i2cHandle);

/*
 * DATA AQUISITION
 */
HAL_StatusTypeDef MPL3115A2_ReadTemperature(MPL3115A2 *dev);
HAL_StatusTypeDef MPL3115A2_ReadAltitude(MPL3115A2 *dev);
HAL_StatusTypeDef MPL3115A2_ReadPressure(MPL3115A2 *dev);


/*
 * LOW-LEVEL FUNCTIONS
 */
HAL_StatusTypeDef MPL3115A2_ReadRegister(MPL3115A2 *dev, uint8_t reg, uint8_t *data);
HAL_StatusTypeDef MPL3115A2_ReadRegisters(MPL3115A2 *dev, uint8_t reg, uint8_t *data, uint8_t length);
HAL_StatusTypeDef MPL3115A2_WriteRegister(MPL3115A2 *dev, uint8_t reg, uint8_t *data);



#endif
