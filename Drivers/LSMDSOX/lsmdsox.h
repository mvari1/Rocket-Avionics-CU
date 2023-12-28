/*
 *
 * 	LSMDSOX Accelerometer I2C Driver 
 *
 *	Author:	Mason Vari
 *	Created: 22 December 2023
 *
 */


/* Prevents recursive inclusion -------------------------------------*/
#ifndef LSMDSOX_I2C_DRIVER_H
#define LSMDSOX_I2C_DRIVER_H


/* Includes ---------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/*
 * DEFINES (pg. 55)
 */
#define LSMDSOX_I2C_ADDR		(0x6A << 1 )  /* SAD = 0 -> 0xD4 = (0x6A << 1), SAD = 1 -> 0xD6  = (0x6B << 1) (p. 19)*/
#define WHO_AM_I				0x0F	
#define LSMDSOX_DEVICE_ID		0x6C


/*
 * CONTROL REGISTERS  (pg. 44)
 */
#define FIFO_CTRL1				0x07
#define FIFO_CTRL2				0x08
#define FIFO_CTRL3				0x09
#define FIFO_CTRL4				0x0A

#define INT1_CTRL 				0x0D
#define INT2_CTRL				0x0E

#define CTRL1_XL				0x10
#define CTRL2_G					0x11
#define CTRL3_C					0x12
#define CTRL4_C					0x13
#define CTRL5_C					0x14
#define CTRL6_C					0x15
#define CTRL7_G					0x16
#define CTRL8_XL				0x17
#define CTRL9_XL				0x18
#define CTRL10_C				0x19

#define UI_CTRL1_OIS			0x70


/*
 * STATUS REGISTER (pg. 44)
 */
#define STATUS_REG 				0x1E
#define STATUS_MASTER_MAINPAGE	0x39
#define FIFO_STATUS1			0x3A
#define FIFO_STATUS2			0x3B



/*
 * OUTPUT REGISTERS (pg. 45)
 */
#define OUT_TEMP_L				0x20
#define OUT_TEMP_H				0x21

#define OUTX_L_G				0x22
#define OUTX_H_G				0x23
#define OUTY_L_G				0x24
#define OUTY_H_G				0x25
#define OUTZ_L_G				0x26
#define OUTZ_H_G				0x27

#define OUTX_L_A				0x28
#define OUTX_H_A				0x29
#define OUTY_L_A				0x2A
#define OUTY_H_A				0x2B
#define OUTZ_L_A				0x2C
#define OUTZ_H_A				0x2D

#define UI_OUTX_L_G_OIS			0x4A



/*
 * INTERRUPT REGISTERS
 */


/*
 * SENSOR STRUCT
 */

typedef struct {

	/* I2C handle */
	I2C_HandleTypeDef *i2cHandle;

	float_t accelerometer_x;
	float_t accelerometer_y;
	float_t accelerometer_z;

	float_t gyroscrope_x;
	float_t gyroscrope_y;
	float_t gyroscrope_z;

	float_t temp_c;

	


} LSMDSOX;

/*
 * INITIALIZATION
 */
uint8_t	LSMDSOX_Initialize(LSMDSOX *dev, I2C_HandleTypeDef *i2cHandle);

/*
 * DATA AQUISITION
 */
HAL_StatusTypeDef LSMDSOX_ReadTemperature(LSMDSOX *dev);
HAL_StatusTypeDef LSMDSOX_ReadAccelerometer(LSMDSOX *dev);
HAL_StatusTypeDef LSMDSOX_ReadGyroscope(LSMDSOX *dev);


/*
 * LOW-LEVEL FUNCTIONS
 */
HAL_StatusTypeDef LSMDSOX_ReadRegister(LSMDSOX *dev, uint8_t reg, uint8_t *data);
HAL_StatusTypeDef LSMDSOX_ReadRegisters(LSMDSOX *dev, uint8_t reg, uint8_t *data, uint8_t length);
HAL_StatusTypeDef LSMDSOX_WriteRegister(LSMDSOX *dev, uint8_t reg, uint8_t *data);



#endif
