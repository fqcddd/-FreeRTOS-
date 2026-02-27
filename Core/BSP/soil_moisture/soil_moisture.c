//
// Created by keysk on 2025/11/20.
//

#include "soil_moisture.h"
#include "adc.h"
#include "global/adc_buffer.h"

void SoilMoisture_init(void) {
    //留空即可，ADC和DMA会在 SensorTask 中统一启动
}

/**
 * 获得土壤湿度值
 * 此值仅是土壤湿度大时导致传感器电阻减小
 * 随后测量到的传感器的电压值
 * 仅能相对地展示土壤湿度的大小与变化
 * @return 0~100的土壤湿度值
 */
uint16_t SoilMoisture_Get() {
    uint16_t adc = adc_buffer[ADC_INDEX_SOIL_MOISTURE];
    if (adc > 4000) {
        adc = 4000;
    }
    return 100 - adc / 40;
}
