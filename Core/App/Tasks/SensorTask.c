/**
 * @file SensorTask.c
 * @brief 传感器数据采集和报警检测任务
 *
 * 本任务负责：
 * 1. 初始化所有传感器（AHT20温湿度、土壤湿度、光照、降雨量）
 * 2. 定期读取传感器数据并更新全局状态
 * 3. 检查环境参数是否超出安全范围
 * 4. 当检测到异常时，发送报警消息到BLE队列并启动蜂鸣器
 *
 * 任务优先级：osPriorityNormal
 * 任务周期：1000ms（1秒）
 */

#include "aht20.h"
#include "cmsis_os2.h"
#include "freertos.h"
#include "global/farmState.h"
#include "light.h"
#include "main.h"
#include "rain.h"
#include "soil_moisture.h"
#include "pump.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "BeepTimer.h"
#include "global/adc_buffer.h"
#include "adc.h"
#include "screen.h"
#include "BMP280.h"

/**
 * @brief 发送浮点数类型的报警消息
 *
 * 分配内存创建JSON格式的报警消息，并通过队列发送给BLETask
 * 消息格式：{"type":"warning", "reason":"报警原因", "value":数值}
 *
 * @param reason 报警原因字符串（如"temperature_high"）
 * @param value 报警时的数值（浮点数）
 *
 * @note 使用pvPortMalloc分配内存，BLETask负责释放
 */
static void SendWarningFloat(const char *reason, float value) {
  // 在FreeRTOS堆中分配内存用于存储消息
  char *msg = pvPortMalloc(100);
  if (msg == NULL) {
    return; // 内存分配失败，直接返回
  }

  // 将浮点数转换为整数和小数部分
  int minInt, minDec;
  floatToIntDec(value, &minInt, &minDec);

  // 格式化JSON消息
  snprintf(msg, 100, "{\"type\":\"warning\", \"reason\":\"%s\", \"value\":%d.%d}", reason, minInt, minDec);

  // 将消息指针放入BLE队列，等待BLETask处理
  osMessageQueuePut(BLEQueueHandle, &msg, 0, 0);
}

/**
 * @brief 发送整数类型的报警消息
 *
 * 分配内存创建JSON格式的报警消息，并通过队列发送给BLETask
 * 消息格式：{"type":"warning", "reason":"报警原因", "value":数值}
 *
 * @param reason 报警原因字符串（如"soil_moisture_low"）
 * @param value 报警时的数值（整数）
 *
 * @note 使用pvPortMalloc分配内存，BLETask负责释放
 */
static void SendWarningInt(const char *reason, int value) {
  // 在FreeRTOS堆中分配内存用于存储消息
  char *msg = pvPortMalloc(100);
  if (msg == NULL) {
    return; // 内存分配失败，直接返回
  }

  // 格式化JSON消息
  snprintf(msg, 100, "{\"type\":\"warning\", \"reason\":\"%s\", \"value\":%d}", reason, value);

  // 将消息指针放入BLE队列，等待BLETask处理
  osMessageQueuePut(BLEQueueHandle, &msg, 0, 0);
}

/**
 * @brief 检查浮点数是否在安全范围内
 *
 * 如果值超出范围，发送相应的报警消息
 *
 * @param value 要检查的浮点数值
 * @param min 最小值阈值
 * @param max 最大值阈值
 * @param minReason 低于最小值时的报警原因字符串
 * @param maxReason 高于最大值时的报警原因字符串
 * @return 返回1表示有报警，返回0表示正常
 */
static uint8_t CheckRangeFloat(float value, float min, float max, const char *minReason, const char *maxReason) {
  uint8_t warning = 0;

  // 检查是否低于最小值
  if (value < min) {
    SendWarningFloat(minReason, value);
    warning = 1;
  }

  // 检查是否高于最大值
  if (value > max) {
    SendWarningFloat(maxReason, value);
    warning = 1;
  }

  return warning;
}

/**
 * @brief 检查整数是否在安全范围内
 *
 * 如果值超出范围，发送相应的报警消息
 *
 * @param value 要检查的整数值
 * @param min 最小值阈值
 * @param max 最大值阈值
 * @param minReason 低于最小值时的报警原因字符串
 * @param maxReason 高于最大值时的报警原因字符串
 * @return 返回1表示有报警，返回0表示正常
 */
static uint8_t CheckRangeInt(int value, int min, int max, const char *minReason, const char *maxReason) {
  uint8_t warning = 0;

  // 检查是否低于最小值
  if (value < min) {
    SendWarningInt(minReason, value);
    warning = 1;
  }

  // 检查是否高于最大值
  if (value > max) {
    SendWarningInt(maxReason, value);
    warning = 1;
  }

  return warning;
}

/**
 * @brief 传感器任务主函数
 *
 * 任务执行流程：
 * 1. 初始化环境安全范围阈值
 * 2. 初始化所有传感器（需要互斥锁保护I2C总线）
 * 3. 进入主循环：
 *    - 读取所有传感器数据
 *    - 更新全局状态变量
 *    - 检查各参数是否超出安全范围
 *    - 如有异常，发送报警并启动蜂鸣器
 *    - 延时1秒后继续下一次循环
 *
 * @param argument 任务参数（未使用）
 *
 * @note
 * - 使用i2c1Mutex互斥锁保护I2C1总线，因为OLED和AHT20共享I2C1
 * - 任务周期为1秒，保证数据更新及时且不会过于频繁
 */
void StartSensorTask(void *argument) {
  // 初始化环境安全范围阈值（设置默认值）
  EnvSafeRange_Init();

  // 初始化AHT20温湿度传感器（需要I2C通信，使用互斥锁保护）
  osMutexAcquire(i2c2MutexHandle, osWaitForever);
  AHT20_Init();
  osMutexRelease(i2c2MutexHandle);

  osMutexAcquire(i2c2MutexHandle, osWaitForever);
  BMP280_Init();
  osMutexRelease(i2c2MutexHandle);

  // 初始化其他传感器（使用ADC，不需要I2C）
  //Rain_init();           // 降雨量传感器
  //SoilMoisture_init();   // 土壤湿度传感器
  // 【核心新增】：一键启动 ADC1 多通道 DMA 循环搬运！
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_buffer, ADC_CHANNEL_COUNT);
  // 加一个计数器，决定多长时间记录一个点
  // 假设任务是 1 秒循环一次，如果要记录过去 2 小时的数据，
  // 120 分钟 / 128 个点 ≈ 差不多每 1 分钟（60秒）记录一次。
  // 这里为了你方便看效果，我们先设为每 1 秒记录一次！
  static uint8_t record_timer = 0;
  // 主循环：定期采集传感器数据并检测报警
  for (;;) {
    // 读取AHT20温湿度传感器数据（需要I2C通信，使用互斥锁保护）
    osMutexAcquire(i2c2MutexHandle, osWaitForever);
    AHT20_Read(&farmState.temperature, &farmState.humidity);
    osMutexRelease(i2c2MutexHandle);
    osMutexAcquire(i2c2MutexHandle, osWaitForever);
    // 【关键顺序】：必须先读温度！因为底层的 t_fine 变量靠它更新！
    farmState.bmp_temp = BMP280_ReadTemp();
    osMutexRelease(i2c2MutexHandle);
    osMutexAcquire(i2c2MutexHandle, osWaitForever);
    // 有了新鲜的 t_fine，此时测气压和海拔才是绝对精准的
    farmState.pressure = BMP280_ReadPress();
    osMutexRelease(i2c2MutexHandle);

    // 读取其他传感器数据（使用ADC，不需要互斥锁）
    farmState.rainGauge = Rain_Get();           // 读取降雨量
    farmState.soilMoisture = SoilMoisture_Get(); // 读取土壤湿度
    farmState.lightIntensity = Light_Get();     // 读取光照强度

    // 【核心新增】：周期性把数据压入环形缓冲区
    record_timer++;
    if (record_timer >= 1) { // 1秒记录一次（后期可改成 60 就是一分钟记一次）
      // 写入当前土壤湿度
      soilHistory.buffer[soilHistory.head_index] = farmState.soilMoisture;
      // 游标往前推一步。如果到了 128，就自动回到 0，覆盖最老的数据
      soilHistory.head_index = (soilHistory.head_index + 1) % HISTORY_MAX_LEN;

      // 2. 【新增】：记录降雨量
      rainHistory.buffer[rainHistory.head_index] = farmState.rainGauge;
      rainHistory.head_index = (rainHistory.head_index + 1) % HISTORY_MAX_LEN;

      // 3. 【新增】：记录光照历史
      lightHistory.buffer[lightHistory.head_index] = farmState.lightIntensity;
      lightHistory.head_index = (lightHistory.head_index + 1) % HISTORY_MAX_LEN;

      record_timer = 0;
    }

    // 根据土壤湿度自动控制水泵
    // 当土壤湿度低于最低阈值时打开水泵，高于最低阈值时关闭水泵
    if (farmState.soilMoisture < farmSafeRange.minSoilMoisture) {
      // 土壤湿度低于最低阈值，打开水泵
      Pump_On();
      farmState.waterPumpState = 1;
    } else {
      // 土壤湿度高于或等于最低阈值，关闭水泵
      Pump_Off();
      farmState.waterPumpState = 0;
    }

    // 检查各环境参数是否超出安全范围，累计报警标志
    uint8_t warning = 0;
    warning += CheckRangeFloat(farmState.temperature, farmSafeRange.minTemperature,
                               farmSafeRange.maxTemperature, "temperature_low", "temperature_high");
    warning += CheckRangeFloat(farmState.humidity, farmSafeRange.minHumidity,
                               farmSafeRange.maxHumidity, "humidity_low", "humidity_high");

    // 降雨量只有上限检查（超过阈值才报警）
    if (farmState.rainGauge > farmSafeRange.maxRainGauge) {
      warning += 1;
      SendWarningInt("rain_gauge_high", farmSafeRange.maxRainGauge);
    }

    warning += CheckRangeInt(farmState.soilMoisture, farmSafeRange.minSoilMoisture,
                             farmSafeRange.maxSoilMoisture, "soil_moisture_low", "soil_moisture_high");
    warning += CheckRangeInt(farmState.lightIntensity, farmSafeRange.minLightIntensity,
                             farmSafeRange.maxLightIntensity, "light_intensity_low", "light_intensity_high");

    // 根据报警标志控制蜂鸣器
    if (warning > 0) {
      Beep_on();  // 有报警，启动蜂鸣器
    } else {
      Beep_off(); // 无报警，关闭蜂鸣器
    }

    // 延时1秒后继续下一次数据采集
    osDelay(1000);
  }
}