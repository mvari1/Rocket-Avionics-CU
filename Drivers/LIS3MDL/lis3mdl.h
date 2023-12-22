/*
 *
 * 	LIS3MDL Magnetometer I2C Driver
 *
 *	Author:	Mason Vari
 *	Created: 12 October 2023
 *
 */

/* Prevents recursive inclusion -------------------------------------*/
#ifndef LIS3MDL_I2C_DRIVER_H
#define LIS3MDL_I2C_DRIVER_H


/* Includes ---------------------------------------------------------*/
#include "stm32l4xx_hal.h"



/*
 * DEFINES
 */
#define LIS3MDL_I2C_ADDR		(0x1C << 1) /* SAD = 0 -> 0x38 = (0x1C << 1), SAD = 1 -> 0x3C  = (0x1E << 1) (p. 17)*/

#define WHO_AM_I				0x0F
#define LIS3MDL_DEVICE_ID		0x3D


/*
 * CONTROL REGISTERS (p. 23)
 */
#define CTRL_REG1				0x20
#define CTRL_REG2				0x21
#define CTRL_REG3				0x22
#define CTRL_REG4				0x23
#define CTRL_REG5				0x24

/*
 * STATUS REGISTER
 */
#define STATUS_REG				0x27

/*
 * OUTPUT REGISTERS
 */
#define OUT_X_L					0x28
#define OUT_X_H					0x29
#define OUT_Y_L					0x2A
#define OUT_Y_H					0x2B
#define OUT_Z_L					0x2C
#define OUT_Z_H					0x2D
#define TEMP_OUT_L				0x2E
#define TEMP_OUT_H				0x2F

/*
 * INTERRUPT REGISTERS
 */
#define INT_CFG					0x30
#define INT_SRC					0x31
#define INT_THS_L				0x32
#define INT_THS_H				0x33

/*
 * SENSOR STRUCT
 */

typedef struct {

	/* I2C handle */
	I2C_HandleTypeDef *i2cHandle;

	/* Magnetometer dta (X, Y, Z) in gauss */
	float mag_guass[3];

	/* Temperature data in celsius */
	float temp_c;

} LIS3MDL;

/*
 * INITIALIZATION
 */
uint8_t	LIS3MDL_Initialize(LIS3MDL *dev, I2C_HandleTypeDef *i2cHandle);

/*
 * DATA AQUISITION
 */
HAL_StatusTypeDef LIS3MDL_ReadTemperature(LIS3MDL *dev);
HAL_StatusTypeDef LIS3MDL_ReadMagnetometer(LIS3MDL *dev);


/*
 * LOW-LEVEL FUNCTIONS
 */
HAL_StatusTypeDef LIS3MDL_ReadRegister(LIS3MDL *dev, uint8_t reg, uint8_t *data);
HAL_StatusTypeDef LIS3MDL_ReadRegisters(LIS3MDL *dev, uint8_t reg, uint8_t *data, uint8_t length);

HAL_StatusTypeDef LIS3MDL_WriteRegister(LIS3MDL *dev, uint8_t reg, uint8_t *data);


#endif








