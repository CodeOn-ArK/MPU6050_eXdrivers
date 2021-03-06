/*
 * MPU60x0.c
 *
 *  Created on: 24-Jun-2021
 *      Author: ark
 */

#include "MPU60x0.h"
#include <stdlib.h>
#include "stm32f446xx_i2c.h"

extern I2C_Handle_t I2C2Handle;


/*
 * @Descp : Wrapper function for I2C_MasterSenData
 *
 * @Params:
 *
 * @Note  :
 */
void MPU_Write(uint8_t *data, uint8_t count)
{
	I2C_MasterSendData( &I2C2Handle, data, count, MPU_ADDR);
}

/*
 * @Descp : Used specifically to send address
 *
 * @Params: The address code of the concerned register
 *
 * @Note  : NULL
 */
void MPU_SendAddress(uint8_t Addr)
{
	I2C_MasterSendData( &I2C2Handle, &Addr, 1, MPU_ADDR);
}

/*
 * @Descp : Wrapper for I2C_MasterReceiveData
 *
 * @Params:
 *
 * @Note  :
 */
void MPU_Read(uint8_t *data, uint8_t count)
{
	I2C_MasterReceiveData(&I2C2Handle, data, count, MPU_ADDR);
}

/*
 * @Descp : The Sample Rate for GYRO is generated by dividing the Gyro o/p rate by SMPLRT_DIV(divval)
 *  		Sample Rate = Gyro Output Rate /(1 + divval)
 *  		Gyro output rate = 8KHz when DLPF disabled else 1KHz
 *
 * @Params: any value from 0-255
 *
 * @Note  :	Accelerometer Sample Rate is 1KHz
 */
void MPU_Sample_Rate_Divider(uint8_t divval){
	uint8_t tempdata[2] = {0,0};

	tempdata[0] = SAMPLE_RATE_DIVIDER;
	tempdata[1] = divval;

	MPU_Write(tempdata , 2);
}


/*
 * @Descp : This is used to configure the DLPF(Digital Low Pass Filter)
 *
 * @Params: ConfigVal, any value between 0 - 7
 *
 * @Note  : NULL
 */
void MPU_Config_Register(uint8_t ConfigVal){
	uint8_t tempdata[2] = {0,0};

	tempdata[0] = CONFIG_REG;
	tempdata[1] = ConfigVal;

	MPU_Write(tempdata , 2);
}


/*
 * @Descp : Selects the  Full scale output of the Gyro sensor
 *
 * @Params: @GFscale
 *
 * @Note  : NULL
 */
void MPU_Gyro_FullScale_Selection(uint8_t Scale){

	uint8_t tempdata[2] = {0,0};

	tempdata[0] = GYRO_CONFIG;
	tempdata[1] = Scale;

	MPU_Write(tempdata , 2);
}


/*
 * @Descp : Selects the Full scale output of the Accelerometer sensor
 *
 * @Params: @AFScale
 *
 * @Note  : NULL
 */
void MPU_Accel_FullScale_Selection(uint8_t Scale){
	uint8_t tempdata[2] = {0,0};

	tempdata[0] = ACCEL_CONFIG;
	tempdata[1] = Scale;

	MPU_Write(tempdata , 2);
}


/*
 * @Descp : This register determines which sensor measurements are loaded into the FIFO buffer
 *
 * @Params: @FIFOEN
 *
 * @Note  : NULL
 */
void MPU_FIFO_Enable(uint8_t BitVal){			//Accepts bit values of the sensors, for which FIFO needs to be enabled
	uint8_t tempdata[2] = {0,0};

	tempdata[0] = FIFO_ENABLE;
	tempdata[1] = BitVal;

	MPU_Write(tempdata , 2);
}


/*
 * @Descp : This function configs the behavior of the interrupt signals at the INT pin
 *
 * @Params: @INTCONFIG
 *
 * @Note  : NULL
 */
void MPU_Interrupt_Pin_Config(uint8_t BitVal){
	uint8_t tempdata[2] = {0,0};

	tempdata[0] = INT_PIN_CONFIG;
	tempdata[1] = BitVal;

	MPU_Write(tempdata , 2);
}


/*
 * @Descp : Enables interrupt generation by interrupt sources
 *
 * @Params: @INTEN
 *
 * @Note  : NULL
 */
void MPU_Interrupt_Enable(uint8_t BitVal){
	uint8_t tempdata[2] = {0,0};

	tempdata[0] = INT_ENABLE;
	tempdata[1] = BitVal;

	MPU_Write(tempdata , 2);
}


/*
 * @Descp : Shows the status of the specified interrupt generating source
 *
 * @Params: @INTSTAT
 *
 * @Note  : Returns interrupt status of the corresponding flag of INT_STATUS register
 */
uint8_t MPU_Get_Interrupt_Status(uint8_t Flag){  	//Checks the given flag with the flag register of the InterruptStatus Register
	uint8_t tempdata[2] = {0,0};

	tempdata[0] = INT_STATUS;
	tempdata[1] = Flag;

	MPU_Write(tempdata , 2);
	MPU_Read(tempdata, 1);

	return tempdata[0];
}


/*
 * @Descp :
 *
 * @Params:
 *
 * @Note  : NULL
 */
Cord_RegDef_t MPU_Get_Accel_value(void){
	Cord_RegDef_t AccelVal;
	memset(&AccelVal, 0, sizeof(AccelVal));

	MPU_SendAddress(ACCEL_XOUT_H);
	MPU_Read(&AccelVal.X_Raw_H, 1);

	MPU_SendAddress(ACCEL_XOUT_L);
	MPU_Read(&AccelVal.X_Raw_L, 1);

	MPU_SendAddress(ACCEL_YOUT_H);
	MPU_Read(&AccelVal.Y_Raw_H, 1);

	MPU_SendAddress(ACCEL_YOUT_L);
	MPU_Read(&AccelVal.Y_Raw_L, 1);

	MPU_SendAddress(ACCEL_ZOUT_H);
	MPU_Read(&AccelVal.Z_Raw_H, 1);

	MPU_SendAddress(ACCEL_ZOUT_L);
	MPU_Read(&AccelVal.Z_Raw_L, 1);

	return AccelVal;
}


/*
 * @Descp :
 *
 * @Params:
 *
 * @Note  : NULL
 */
Cord_RegDef_t MPU_Get_Gyro_value(void){
	Cord_RegDef_t GyroVal;
	memset(&GyroVal, 0, sizeof(GyroVal));

	MPU_SendAddress(ACCEL_XOUT_H);
	MPU_Read(&GyroVal.X_Raw_H, 1);

	MPU_SendAddress(ACCEL_XOUT_L);
	MPU_Read(&GyroVal.X_Raw_L, 1);

	MPU_SendAddress(ACCEL_YOUT_H);
	MPU_Read(&GyroVal.Y_Raw_H, 1);

	MPU_SendAddress(ACCEL_YOUT_L);
	MPU_Read(&GyroVal.Y_Raw_L, 1);

	MPU_SendAddress(ACCEL_ZOUT_H);
	MPU_Read(&GyroVal.Z_Raw_H, 1);

	MPU_SendAddress(ACCEL_ZOUT_L);
	MPU_Read(&GyroVal.Z_Raw_L, 1);

	return GyroVal;
}


/*
 * @Descp :
 *
 * @Params:
 *
 * @Note  : NULL
 */
Temp_RegDef_t MPU_Get_Temp(void){
	Temp_RegDef_t TempReg;

	MPU_SendAddress(TEMP_OUT_H);
	MPU_Read(&TempReg.H_Raw, 1);

	MPU_SendAddress(TEMP_OUT_L);
	MPU_Read(&TempReg.L_Raw, 1);

	return TempReg;
}


/*
 * @Descp :
 *
 * @Params:
 *
 * @Note  : NULL
 */
void MPU_Sig_Path_Reset(uint8_t whom_to_reset){
	uint8_t tempdata[2] = {0,0};

	tempdata[0] = SIG_PATH_RESET;
	tempdata[1] = whom_to_reset;

	MPU_Write(tempdata , 2);
}


/*
 * @Descp :
 *
 * @Params:
 *
 * @Note  : NULL
 */
void MPU_User_Control(uint8_t BitVal){
	uint8_t tempdata[2] = {0,0};

	tempdata[0] = USER_CTRL;
	tempdata[1] = BitVal;

	MPU_Write(tempdata , 2);
}


/*
 * @Descp :
 *
 * @Params:
 *
 * @Note  : NULL
 */
void MPU_Power_Manager(uint8_t BitVal, uint8_t RegNo){
	uint8_t temp[2] = {0,0};

	if(RegNo == 1){
		temp[0] = PWR_MGMNT_1;
		temp[1] = BitVal;

		MPU_Write(temp, 2);

	}else if(RegNo == 2){
		temp[0] = PWR_MGMNT_2;
		temp[1] = BitVal;

		MPU_Write(temp, 2);

	}

}


/*
 * @Descp :
 *
 * @Params:
 *
 * @Note  : NULL
 */
uint16_t MPU_Get_FIFO_Count(void){
	//MPU_SendAddress();
}


/*
 * @Descp :
 *
 * @Params:
 *
 * @Note  : NULL
 */
uint8_t MPU_Get_FIFO_Data(void){
	MPU_SendAddress(FIFO_R_W);
}


/*
 * @Descp :
 *
 * @Params:
 *
 * @Note  : NULL
 */
uint8_t MPU_WHO_AM_I(void){
	uint8_t temp = 0;

	MPU_SendAddress(WHO_AM_I);
	MPU_Read(&temp,	1);

	return temp;
}


