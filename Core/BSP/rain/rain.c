//
// Created by keysk on 2025/11/20.
//

#include "rain.h"
#include "adc.h"
#include "global/adc_buffer.h"

void Rain_init(void) {
    //留空即可，ADC和DMA会在 SensorTask 中统一启动
}

/**
 * 获得下雨大小
 * 此值仅是雨水打在传感器上，导致传感器电阻减小
 * 随后测量到的传感器的电压值
 * 仅能相对地展示降雨大小，无法精确地表示降雨量
 * @return 0~100的降雨值，0表示无雨，100表示大雨
 */
uint16_t Rain_Get() {
    uint16_t adc = adc_buffer[ADC_INDEX_RAIN];
    if (adc > 4000) {
        adc = 4000;
    }
    return 100 - adc / 40;
}
