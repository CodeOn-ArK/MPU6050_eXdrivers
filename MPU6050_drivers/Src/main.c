/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Auto-generated by STM32CubeIDE
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/*
 * I2C peripheral initialization:
 *
 * SDA: PB9
 * SCL: PB6
 *
 * AF: 4
 */

#if 1
#include "main.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif

void delay();
void I2C1_GPIOInits();
void I2C1_Inits();

extern void initialise_monitor_handles(void);
void MPU_Write(uint8_t *data, uint8_t count);
void MPU_Read(uint8_t *data, uint8_t count);

#define MASTER_ADDR		 	 0x38		//Set the master address here
#define MPU_ADDR			 0x68		//Set the slave address here
#define SDA_PIN				 GPIO_PIN_3
#define SCL_PIN				 GPIO_PIN_10

uint8_t data[] = "Testing I2C @#??";

//global I2C handle variable
I2C_Handle_t I2C1Handle;


void I2C1_GPIOInits()
{
	//Initialize the GPIO pins to be used with I2C peripheral
	GPIO_handle_t 	I2CPins;

	I2CPins.pGPIOx = GPIOB;
	I2CPins.GPIO_PinConfig.GPIO_PinMode = GPIO_ALT_FN;
	I2CPins.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
	I2CPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_LOW;
	I2CPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	I2CPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;			//Always keep this pulled up

	//SDA
	I2CPins.GPIO_PinConfig.GPIO_PinNumber  =  SDA_PIN;
	GPIO_Init(&I2CPins);

	//SCL
	I2CPins.GPIO_PinConfig.GPIO_PinNumber  =  SCL_PIN;
	GPIO_Init(&I2CPins);



}

void I2C1_Inits()
{

	//Initiliaze the I2C peripheral
	I2C1Handle.pI2Cx  = I2C2;
	I2C1Handle.I2C_Config.I2C_ACKControl = I2C_ACK_EN;
	I2C1Handle.I2C_Config.I2C_DeviceAddress  =  MASTER_ADDR;
	I2C1Handle.I2C_Config.I2C_FMDutyCycle = I2C_FM_DUTY_2;
	I2C1Handle.I2C_Config.I2C_SCLSpeed = I2C_SCL_SPEED_FM4K;

	I2C_Init(&I2C1Handle);
}

int main()
{
	initialise_monitor_handles();

	//GPIO Button Init
	GPIO_ButtonConfig();

	//I2C pin initsI2C1Handle
	I2C1_GPIOInits();

	//I2C peripheral config
	I2C1_Inits();

	//Enable the Peripheral
	I2C_Enable( I2C2 , ENABLE);

	printf("%s", "Hellos\n\r");

	uint8_t data[3] = {0,0,0};
	uint16_t full_data = 0;

	while(1)
	{
		if( GPIO_ReadIPin(GPIOC, GPIO_PIN_3) == GPIO_PIN_RESET)
		{//Send data
			delay();

			data[0] = 0x75;
			I2C_MasterSendData(&I2C1Handle, &data[0], 1, MPU_ADDR);
			I2C_MasterReceiveData(&I2C1Handle, &data[0], 1, MPU_ADDR);

			data[0] = 0x68;
			data[1] = 0x1;
			I2C_MasterSendData(&I2C1Handle, data, 2, MPU_ADDR);


			data[0] = 0x6B;
			data[1] = 0x0;
			I2C_MasterSendData(&I2C1Handle, data, 2, MPU_ADDR);

			uint8_t temp = 0;

			data[0] = 0x1C;		//Accelerometer Config set AFS
			data[1] = (0x3 << 3);
			I2C_MasterSendData( &I2C1Handle, data, 2, MPU_ADDR);

			/*
			 * Start Reading Accelerometer sensor data
			 */
			data[0] = 0x3B; //XOUT_H
			I2C_MasterSendData( &I2C1Handle, &data[0], 1, MPU_ADDR);
			I2C_MasterReceiveData(&I2C1Handle, &data[1], 1, MPU_ADDR);
			data[0] = 0x3C; //XOUT_L
			I2C_MasterSendData( &I2C1Handle, &data[0], 1, MPU_ADDR);
			I2C_MasterReceiveData(&I2C1Handle, &data[2], 1, MPU_ADDR);

			data[0] = 0x11;
			printf("%x\t %x\t %x\t %x\n\r",data[0], *(data + 1), *(data + 2), *((uint16_t *)(data + 1)));
			temp = data[1];
			data[1] = data[2];
			data[2] = temp;
			printf("%d\n\r", *((uint16_t*)&data[1]));
			//full_data |= ((uint16_t *)data);
			//printf("%d\n\r", full_data);

			data[0] = 0x3D; //YOUT_H
			I2C_MasterSendData( &I2C1Handle, &data[0], 1, MPU_ADDR);
			I2C_MasterReceiveData(&I2C1Handle, &data[1], 1, MPU_ADDR);
			data[0] = 0x3E; //YOUT_L
			I2C_MasterSendData( &I2C1Handle, &data[0], 1, MPU_ADDR);
			I2C_MasterReceiveData(&I2C1Handle, &data[2], 1, MPU_ADDR);

			data[0] = 0x11;
			printf("%x\t %x\t %x\t %x\n\r",data[0], *(data + 1), *(data + 2), *((uint16_t *)(&data[1])));
			temp = data[1];
			data[1] = data[2];
			data[2] = temp;
			printf("%d\n\r", *((uint16_t*)&data[1]));

			data[0] = 0x3F; //ZOUT_H
			I2C_MasterSendData( &I2C1Handle, &data[0], 1, MPU_ADDR);
			I2C_MasterReceiveData(&I2C1Handle, &data[1], 1, MPU_ADDR);
			data[0] = 0x40; //ZOUT_L
			I2C_MasterSendData( &I2C1Handle, &data[0], 1, MPU_ADDR);
			I2C_MasterReceiveData(&I2C1Handle, &data[2], 1, MPU_ADDR);

			data[0] = 0x11;
			printf("%x\t %x\t %x\t %x\n\r",data[0], *(data + 1), *(data + 2), *((uint16_t *)(data + 1)));
			temp = data[1];
			data[1] = data[2];
			data[2] = temp;
			printf("%d\n\r", *((uint16_t*)&data[1]));

			printf("\033[0;0H \033[2J");
			GPIO_ToggleOPin(GPIOA, GPIO_PIN_5);
		}
	}
}
#endif

void MPU_Write(uint8_t *data, uint8_t count)
{
	I2C_MasterSendData( &I2C1Handle, data, count, MPU_ADDR);
}
void MPU_Read(uint8_t *data, uint8_t count)
{
	I2C_MasterReceiveData(&I2C1Handle, data, count, MPU_ADDR);
}
