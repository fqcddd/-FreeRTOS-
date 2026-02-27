#include "BMP280.h"
#include "math.h"

BMP280_Calibration BMP280_Cal;	//BMP280校准结构体

/*
 * 函数：BMP280初始化
 * 参数：无
 * 返回：读取ID成功并初始化成功返回0，读取ID错误返回1
 */
GPIO_PinState BMP280_Init(void)
{
	//若读取ID不为0x58，则视为错误
	if(BMP280_ReadID() != 0x58)
	{
		return BMP280_ERROR;
	}


	//初始化校准值
	uint8_t lsb;
	uint8_t msb;

	HAL_I2C_Mem_Read(&BMP280_I2C, BMP280_ADDRESS, BMP280_DIG_T1_LSB_REG, 1, &lsb, 1, BMP280_TIMEOUT);
	HAL_I2C_Mem_Read(&BMP280_I2C, BMP280_ADDRESS, BMP280_DIG_T1_MSB_REG, 1, &msb, 1, BMP280_TIMEOUT);
	BMP280_Cal.T1 = (uint16_t)lsb + ((uint16_t)msb << 8);

	HAL_I2C_Mem_Read(&BMP280_I2C, BMP280_ADDRESS, BMP280_DIG_T2_LSB_REG, 1, &lsb, 1, BMP280_TIMEOUT);
	HAL_I2C_Mem_Read(&BMP280_I2C, BMP280_ADDRESS, BMP280_DIG_T2_MSB_REG, 1, &msb, 1, BMP280_TIMEOUT);
	BMP280_Cal.T2 = (uint16_t)lsb + ((uint16_t)msb << 8);

	HAL_I2C_Mem_Read(&BMP280_I2C, BMP280_ADDRESS, BMP280_DIG_T3_LSB_REG, 1, &lsb, 1, BMP280_TIMEOUT);
	HAL_I2C_Mem_Read(&BMP280_I2C, BMP280_ADDRESS, BMP280_DIG_T3_MSB_REG, 1, &msb, 1, BMP280_TIMEOUT);
	BMP280_Cal.T3 = (uint16_t)lsb + ((uint16_t)msb << 8);

	HAL_I2C_Mem_Read(&BMP280_I2C, BMP280_ADDRESS, BMP280_DIG_P1_LSB_REG, 1, &lsb, 1, BMP280_TIMEOUT);
	HAL_I2C_Mem_Read(&BMP280_I2C, BMP280_ADDRESS, BMP280_DIG_P1_MSB_REG, 1, &msb, 1, BMP280_TIMEOUT);
	BMP280_Cal.P1 = (uint16_t)lsb + ((uint16_t)msb << 8);

	HAL_I2C_Mem_Read(&BMP280_I2C, BMP280_ADDRESS, BMP280_DIG_P2_LSB_REG, 1, &lsb, 1, BMP280_TIMEOUT);
	HAL_I2C_Mem_Read(&BMP280_I2C, BMP280_ADDRESS, BMP280_DIG_P2_MSB_REG, 1, &msb, 1, BMP280_TIMEOUT);
	BMP280_Cal.P2 = (uint16_t)lsb + ((uint16_t)msb << 8);

	HAL_I2C_Mem_Read(&BMP280_I2C, BMP280_ADDRESS, BMP280_DIG_P3_LSB_REG, 1, &lsb, 1, BMP280_TIMEOUT);
	HAL_I2C_Mem_Read(&BMP280_I2C, BMP280_ADDRESS, BMP280_DIG_P3_MSB_REG, 1, &msb, 1, BMP280_TIMEOUT);
	BMP280_Cal.P3 = (uint16_t)lsb + ((uint16_t)msb << 8);

	HAL_I2C_Mem_Read(&BMP280_I2C, BMP280_ADDRESS, BMP280_DIG_P4_LSB_REG, 1, &lsb, 1, BMP280_TIMEOUT);
	HAL_I2C_Mem_Read(&BMP280_I2C, BMP280_ADDRESS, BMP280_DIG_P4_MSB_REG, 1, &msb, 1, BMP280_TIMEOUT);
	BMP280_Cal.P4 = (uint16_t)lsb + ((uint16_t)msb << 8);

	HAL_I2C_Mem_Read(&BMP280_I2C, BMP280_ADDRESS, BMP280_DIG_P5_LSB_REG, 1, &lsb, 1, BMP280_TIMEOUT);
	HAL_I2C_Mem_Read(&BMP280_I2C, BMP280_ADDRESS, BMP280_DIG_P5_MSB_REG, 1, &msb, 1, BMP280_TIMEOUT);
	BMP280_Cal.P5 = (uint16_t)lsb + ((uint16_t)msb << 8);

	HAL_I2C_Mem_Read(&BMP280_I2C, BMP280_ADDRESS, BMP280_DIG_P6_LSB_REG, 1, &lsb, 1, BMP280_TIMEOUT);
	HAL_I2C_Mem_Read(&BMP280_I2C, BMP280_ADDRESS, BMP280_DIG_P6_MSB_REG, 1, &msb, 1, BMP280_TIMEOUT);
	BMP280_Cal.P6 = (uint16_t)lsb + ((uint16_t)msb << 8);

	HAL_I2C_Mem_Read(&BMP280_I2C, BMP280_ADDRESS, BMP280_DIG_P7_LSB_REG, 1, &lsb, 1, BMP280_TIMEOUT);
	HAL_I2C_Mem_Read(&BMP280_I2C, BMP280_ADDRESS, BMP280_DIG_P7_MSB_REG, 1, &msb, 1, BMP280_TIMEOUT);
	BMP280_Cal.P7 = (uint16_t)lsb + ((uint16_t)msb << 8);

	HAL_I2C_Mem_Read(&BMP280_I2C, BMP280_ADDRESS, BMP280_DIG_P8_LSB_REG, 1, &lsb, 1, BMP280_TIMEOUT);
	HAL_I2C_Mem_Read(&BMP280_I2C, BMP280_ADDRESS, BMP280_DIG_P8_MSB_REG, 1, &msb, 1, BMP280_TIMEOUT);
	BMP280_Cal.P8 = (uint16_t)lsb + ((uint16_t)msb << 8);

	HAL_I2C_Mem_Read(&BMP280_I2C, BMP280_ADDRESS, BMP280_DIG_P9_LSB_REG, 1, &lsb, 1, BMP280_TIMEOUT);
	HAL_I2C_Mem_Read(&BMP280_I2C, BMP280_ADDRESS, BMP280_DIG_P9_MSB_REG, 1, &msb, 1, BMP280_TIMEOUT);
	BMP280_Cal.P9 = (uint16_t)lsb + ((uint16_t)msb << 8);

	BMP280_Cal.t_fine = 0;

	//配置寄存器
	uint8_t WriteBuffer = t_0_5ms | Filter_16;//等待时间0.05ms，滤波器等级4
	HAL_I2C_Mem_Write(&BMP280_I2C, BMP280_ADDRESS, BMP280_CONFIG, 1, &WriteBuffer, 1, BMP280_TIMEOUT);

	WriteBuffer = Temp_OverSampl_1|Press_OverSampl_16| Normal_Mode;//过温度采样1，压力过采样8，正常模式
	HAL_I2C_Mem_Write(&BMP280_I2C, BMP280_ADDRESS, BMP280_CTRL_MEAS, 1, &WriteBuffer, 1, BMP280_TIMEOUT);


	return BMP280_OK;
}

/*
 * 函数：BMP280读取设备ID
 * 参数：无
 * 返回：ID值（0x58）
 */
uint8_t BMP280_ReadID(void)
{
	uint8_t ID;


	HAL_I2C_Mem_Read(&BMP280_I2C, BMP280_ADDRESS, BMP280_ID, 1, &ID, 1, BMP280_TIMEOUT);


	return ID;
}

/*
 * 函数：BMP280读取温度值
 * 参数：无
 * 返回：温度值（摄氏度）
 */
double BMP280_ReadTemp(void)
{
	double Temperature;
	int32_t Temp_Reg;

	uint8_t Temp_xlsb;
	uint8_t Temp_lsb;
	uint8_t Temp_msb;

	//I2C读取存储温度数据的寄存器
	HAL_I2C_Mem_Read(&BMP280_I2C, BMP280_ADDRESS, BMP280_TEMP_XLSB, 1, &Temp_xlsb, 1, BMP280_TIMEOUT);
	HAL_I2C_Mem_Read(&BMP280_I2C, BMP280_ADDRESS, BMP280_TEMP_LSB, 1, &Temp_lsb, 1, BMP280_TIMEOUT);
	HAL_I2C_Mem_Read(&BMP280_I2C, BMP280_ADDRESS, BMP280_TEMP_MSB, 1, &Temp_msb, 1, BMP280_TIMEOUT);

	//数据拼接
	Temp_Reg = ((int32_t)Temp_xlsb >> 4) + ((int32_t)Temp_lsb << 4) + ((int32_t)Temp_msb << 12);

	//计算并校准
	Temperature = BMP280_compensate_T(Temp_Reg);

	return Temperature;
}

/*
 * 函数：BMP280读取大气压
 * 参数：无
 * 返回：大气压（Pa）
 */
double BMP280_ReadPress(void)
{
	double Pressure;
	int32_t Press_Reg;

	uint8_t Press_xlsb;
	uint8_t Press_lsb;
	uint8_t Press_msb;

	//I2C读取存储大气压数据的寄存器
	HAL_I2C_Mem_Read(&BMP280_I2C, BMP280_ADDRESS, BMP280_PRESS_XLSB, 1, &Press_xlsb, 1, BMP280_TIMEOUT);
	HAL_I2C_Mem_Read(&BMP280_I2C, BMP280_ADDRESS, BMP280_PRESS_LSB, 1, &Press_lsb, 1, BMP280_TIMEOUT);
	HAL_I2C_Mem_Read(&BMP280_I2C, BMP280_ADDRESS, BMP280_PRESS_MSB, 1, &Press_msb, 1, BMP280_TIMEOUT);

	//数据拼接
	Press_Reg = ((int32_t)Press_xlsb >> 4) + ((int32_t)Press_lsb << 4) + ((int32_t)Press_msb << 12);
	//计算并校准
	Pressure = BMP280_compensate_P(Press_Reg);

	return Pressure;
}

/*
 * 函数：根据BMP280测得的大气压值计算海拔高度
 * 参数：无
 * 返回：海拔高度（m）
 * 注意：误差比较大，不建议使用
 */
double BMP280_ReadAltitude(void)
{
	double Altitude = 0;
	double pressure = 0;

	//测量大气压
	pressure = BMP280_ReadPress();

	//计算海拔高度
	Altitude = 44330 * (1 - pow((pressure/101325.0), 1.0/5.255));

	return Altitude;
}

/*
 * 函数：BMP280温度校准
 * 参数：无
 * 返回：温度值
 */
double BMP280_compensate_T(int adc_T)
{
	double var1, var2, T;
	var1 = (((double)adc_T)/16384.0 - ((double)BMP280_Cal.T1)/1024.0) * ((double)BMP280_Cal.T2);
	var2 = ((((double)adc_T)/131072.0 - ((double)BMP280_Cal.T1)/8192.0) *
	(((double)adc_T)/131072.0 - ((double) BMP280_Cal.T1)/8192.0)) * ((double)BMP280_Cal.T3);
	BMP280_Cal.t_fine = (int)(var1 + var2);
	T = (var1 + var2) / 5120.0;
	return T;
}

/*
 * 函数：BMP280大气压值校准
 * 参数：无
 * 返回：大气压
 */
double BMP280_compensate_P(int adc_P)
{
	double var1, var2, p;
	var1 = ((double)BMP280_Cal.t_fine/2.0) - 64000.0;
	var2 = var1 * var1 * ((double)BMP280_Cal.P6) / 32768.0;
	var2 = var2 + var1 * ((double)BMP280_Cal.P5) * 2.0;
	var2 = (var2/4.0)+(((double)BMP280_Cal.P4) * 65536.0);
	var1 = (((double)BMP280_Cal.P3) * var1 * var1 / 524288.0 + ((double)BMP280_Cal.P2) * var1) / 524288.0;
	var1 = (1.0 + var1 / 32768.0)*((double)BMP280_Cal.P1);
	if (var1 == 0.0){
	    return 0; // avoid exception caused by division by zero
	}
	p = 1048576.0 - (double)adc_P;
	p = (p - (var2 / 4096.0)) * 6250.0 / var1;
	var1 = ((double)BMP280_Cal.P9) * p * p / 2147483648.0;
	var2 = p * ((double)BMP280_Cal.P8) / 32768.0;
	p = p + (var1 + var2 + ((double)BMP280_Cal.P7)) / 16.0;
	return p;
}
