基于STM32F103ZET6开发板使用FreeRTOS系统实现各种传感器模块开发(降雨、土壤、大气压、温度、湿度、光照)，通过OLED实时显示各种参数，并将检测报警信息通过蓝牙与上位机通信

该项目在波特律动stm32C8T6的基础上改动(https://docs.keysking.com/), 使用Clion和cubeMX结合进行开发:

1. 将代码从C8T6移植到ZET6

2. 使用互斥锁继续在I2C2上添加了气压检测

3. 将降雨量传感器和土壤湿度传感器使用同一个ADC进行DMA多通道采样

4. 添加了开机loading动画和各数据历史曲线图

具体接线如下：

AHT20、BMP280和OLED:PB10/I2C2_SCL,PB11/I2C2_SDA

光照传感器:SDAPC4 SCLPC5

土壤湿度传感器:PC2 ADC1_IN12

降雨量传感器:PC1 ADC1 IN11

水泵:PG13

蓝牙:PA2/PA3 (USART2)

编码器:PB6/PB7(TIM4_CH1/TIM4 CH2)

按键: PE4(KEY1) / PE3(KEY3)

蜂鸣器:PA6
