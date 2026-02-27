#ifndef INC_BMP280_H_
#define INC_BMP280_H_

#include "main.h"
#include "i2c.h"

#define BMP280_I2C	hi2c2			//BMP280使用的I2C

//BMP280常用读写寄存器
#define	BMP280_ADDRESS		0xEE	//根据BMP280电路原理图，可能为0xEE或0xEC
#define BMP280_ID			0xD0
#define BMP280_CONFIG		0xF5
#define BMP280_CTRL_MEAS	0xF4
#define BMP280_TEMP_XLSB	0xFC
#define BMP280_TEMP_LSB		0xFB
#define BMP280_TEMP_MSB		0xFA
#define BMP280_PRESS_XLSB	0xF9
#define BMP280_PRESS_LSB	0xF8
#define BMP280_PRESS_MSB	0xF7

//BMP280校准寄存器
#define BMP280_DIG_T1_LSB_REG   0x88
#define BMP280_DIG_T1_MSB_REG   0x89
#define BMP280_DIG_T2_LSB_REG   0x8A
#define BMP280_DIG_T2_MSB_REG   0x8B
#define BMP280_DIG_T3_LSB_REG   0x8C
#define BMP280_DIG_T3_MSB_REG   0x8D
#define BMP280_DIG_P1_LSB_REG   0x8E
#define BMP280_DIG_P1_MSB_REG   0x8F
#define BMP280_DIG_P2_LSB_REG   0x90
#define BMP280_DIG_P2_MSB_REG   0x91
#define BMP280_DIG_P3_LSB_REG   0x92
#define BMP280_DIG_P3_MSB_REG   0x93
#define BMP280_DIG_P4_LSB_REG   0x94
#define BMP280_DIG_P4_MSB_REG   0x95
#define BMP280_DIG_P5_LSB_REG   0x96
#define BMP280_DIG_P5_MSB_REG   0x97
#define BMP280_DIG_P6_LSB_REG   0x98
#define BMP280_DIG_P6_MSB_REG   0x99
#define BMP280_DIG_P7_LSB_REG   0x9A
#define BMP280_DIG_P7_MSB_REG   0x9B
#define BMP280_DIG_P8_LSB_REG   0x9C
#define BMP280_DIG_P8_MSB_REG   0x9D
#define BMP280_DIG_P9_LSB_REG   0x9E
#define BMP280_DIG_P9_MSB_REG   0x9F

//过采样设置,即分辨率设置，过采样率越高，分辨率越高
#define Temp_OverSampl_0	0x00
#define Temp_OverSampl_1	0x20
#define Temp_OverSampl_2	0x40
#define Temp_OverSampl_4	0x60
#define Temp_OverSampl_8	0x80
#define Temp_OverSampl_16	0xE0
#define Press_OverSampl_0	0x00
#define Press_OverSampl_1	0x04
#define Press_OverSampl_2	0x08
#define Press_OverSampl_4	0x0C
#define Press_OverSampl_8	0x10
#define Press_OverSampl_16	0x1C

//模式设置
#define Sleep_Mode	0x00
#define Forced_Mode 0x01
#define Normal_Mode 0x03

//待机时间设置
#define t_0_5ms		0x00
#define t_62_5ms	0x20
#define t_125ms		0x40
#define t_250ms 	0x60
#define t_500ms 	0x80
#define t_1000ms 	0xA0
#define t_2000ms 	0xC0
#define t_4000ms 	0xE0

//滤波器设置
#define Filter_0 0x00
#define Filter_2 0x04
#define Filter_4 0x08
#define Filter_8 0x0C
#define Filter_16 0x1C

//BMP280初始化成功或失败
#define BMP280_OK		0
#define BMP280_ERROR	1

//BMP280 I2C通信等待时间
#define BMP280_TIMEOUT	100

//BMP280校准结构体
typedef struct
{
	uint16_t T1;
	int16_t	T2;
	int16_t	T3;
	uint16_t P1;
	int16_t	P2;
	int16_t	P3;
	int16_t	P4;
	int16_t	P5;
	int16_t	P6;
	int16_t	P7;
	int16_t	P8;
	int16_t	P9;
	int32_t t_fine;
}BMP280_Calibration;


//在主函数中调用的基本函数
GPIO_PinState BMP280_Init(void);
uint8_t BMP280_ReadID(void);
double BMP280_ReadTemp(void);
double BMP280_ReadPress(void);
double BMP280_ReadAltitude(void);

//无需在主函数调用
double BMP280_compensate_T(int adc_T);
double BMP280_compensate_P(int adc_P);


#endif
