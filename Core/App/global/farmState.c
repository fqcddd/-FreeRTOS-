
#include "farmState.h"

extern volatile uint8_t ble_pending_msgs = 0;

// 全局变量定义
// 【修改这里】：暂时赋初值，防止屏幕上全显示 0
FarmState farmState = {
    .temperature = 26.5f,
    .humidity = 55.2f,
    .rainGauge = 12,
    .soilMoisture = 45,
    .lightIntensity = 1024,
    .waterPumpState = 1
};
FarmSafeRange farmSafeRange; // 环境安全范围阈值，用户可通过界面修改

/**
 * @brief 初始化环境安全范围阈值
 *
 * 设置默认的安全范围值，在系统启动时调用一次
 * 这些默认值适用于大多数农作物的生长环境
 *
 * 默认值说明：
 * - 温度范围：15-30℃（适合大多数温室作物）
 * - 湿度范围：20-70%（避免过干或过湿）
 * - 土壤湿度：20-80%（保持适宜的土壤水分）
 * - 光照强度：30-800 lx（适合大多数植物）
 * - 最大降雨量：80%（超过此值触发报警）
 */
void EnvSafeRange_Init() {
    // 温度范围：15-30摄氏度
    farmSafeRange.minTemperature = 15;
    farmSafeRange.maxTemperature = 30;

    // 湿度范围：20-70%
    farmSafeRange.minHumidity = 20;
    farmSafeRange.maxHumidity = 70;

    // 土壤湿度范围：20-80%
    farmSafeRange.minSoilMoisture = 10;
    farmSafeRange.maxSoilMoisture = 50;

    // 光照强度范围：30-800勒克斯
    farmSafeRange.minLightIntensity = 50;
    farmSafeRange.maxLightIntensity = 800;

    // 最大降雨量阈值：80%
    farmSafeRange.maxRainGauge = 30;
}
