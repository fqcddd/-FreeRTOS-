
#ifndef SMARTFARM_FARM_STATE_H
#define SMARTFARM_FARM_STATE_H

#include "cmsis_os2.h"

#include <stdint.h>

/**
 * @file farmState.h
 * @brief 农场环境状态管理模块
 *
 * 本模块定义了农场环境状态的全局数据结构，包括：
 * - 当前环境状态（温度、湿度、土壤湿度、光照强度、降雨量等）
 * - 环境安全范围阈值（用于报警检测）
 *
 * 这些全局变量由SensorTask更新，由ScreenTask读取显示
 */

/**
 * @brief 农场环境状态结构体
 *
 * 存储当前从传感器读取的实时环境数据
 * 由SensorTask定期更新，供其他任务读取使用
 */
typedef struct {
    float temperature;       // 温度（单位：摄氏度）
    float humidity;          // 湿度（单位：百分比，0-100）
    uint16_t rainGauge;      // 降雨量（单位：百分比，0-100）
    uint16_t soilMoisture;   // 土壤湿度（单位：百分比，0-100）
    uint16_t lightIntensity; // 光照强度（单位：勒克斯 lx）
    double pressure;     // 大气压 (Pa)
    double bmp_temp;     // BMP280 内部温度 (通常用做校准参考，也可显示)

    uint8_t waterPumpState;  // 水泵状态：0-关闭, 1-开启
} FarmState;

/**
 * @brief 环境状态安全范围结构体
 *
 * 定义各环境参数的安全范围阈值，用于报警检测
 * 当环境参数超出此范围时，系统会触发报警
 * 用户可以通过界面修改这些阈值
 */
typedef struct {
    float minTemperature;       // 最低温度阈值（单位：摄氏度）
    float maxTemperature;       // 最高温度阈值（单位：摄氏度）
    float minHumidity;          // 最低湿度阈值（单位：百分比）
    float maxHumidity;          // 最高湿度阈值（单位：百分比）
    uint16_t maxRainGauge;      // 最大降雨量阈值（单位：百分比）
    uint16_t minSoilMoisture;   // 最低土壤湿度阈值（单位：百分比）
    uint16_t maxSoilMoisture;   // 最高土壤湿度阈值（单位：百分比）
    uint16_t minLightIntensity; // 最低光照强度阈值（单位：勒克斯）
    uint16_t maxLightIntensity; // 最高光照强度阈值（单位：勒克斯）
} FarmSafeRange;

// 全局变量声明
extern FarmState farmState;        // 当前农场环境状态（由SensorTask更新）
extern FarmSafeRange farmSafeRange; // 环境安全范围阈值（用户可配置）

/**
 * @brief 初始化环境安全范围阈值
 *
 * 设置默认的安全范围值，在系统启动时调用
 * 用户后续可以通过界面修改这些值
 */
void EnvSafeRange_Init();

extern volatile uint8_t ble_pending_msgs ;


#endif //SMARTFARM_FARM_STATE_H
