基于STM32F103ZET6开发板使用FreeRTOS系统实现各种传感器模块开发(降雨、土壤、大气压、温度、湿度、光照)，通过OLED实时显示各种参数，并将检测报警信息通过蓝牙与上位机通信

该项目在波特律动stm32C8T6的基础上改动(https://docs.keysking.com/), 使用Clion和cubeMX结合进行开发

**具体改动和新增如下：**

1. 将代码从C8T6移植到ZET6

2. 使用互斥锁继续在I2C2上添加了气压检测

3. 将降雨量传感器和土壤湿度传感器使用同一个ADC进行DMA多通道采样

4. 添加了开机loading动画和各数据历史曲线图

5. 优化按键机制从轮询模式为中断，从Gpio_input到Gpio_exti,采用二值信号量的方法，当按键按下时释放信号量叫醒InputTask

6. 新增低功耗功能，编写Core/App/StopModeRtc.c文件,采用stopmode设置系统睡眠多少秒。

7. 实现U1动态电源管理：开机后oled显示5s后熄灭系统进入stopmode周期性睡眠，当检测到按键按下时才会重新亮起。

8. 添加Core/Bsp/debug_log/debug_log.c串口打印文件，即检验oled在熄灭时各传感器是否仍然正常工作。

9. 添加蓝牙软件锁和硬件锁，防止蓝牙刚拿到队列消息还没来得及发送，系统就进入睡眠。软件锁为定义全局变量。ble_pending_msgs，当队列拿到消息时ble_pending_msgs++，硬件锁为监控标志位：huart1.gState != HAL_UART_STATE_READY || huart2.gState != HAL_UART_STATE_READY和while (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_TC) == RESET) {}。

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

串口：PA9,PA10
