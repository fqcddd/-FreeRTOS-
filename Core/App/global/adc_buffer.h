/**
* @file adc_buffer.h
 * @brief 全局 ADC DMA 数据缓冲区声明
 */
#ifndef GLOBAL_ADC_BUFFER_H
#define GLOBAL_ADC_BUFFER_H

#include <stdint.h>

// ADC 通道在数组中的索引映射 (Rank 1 和 Rank 2)
#define ADC_INDEX_RAIN           1   // PC2 - 土壤湿度
#define ADC_INDEX_SOIL_MOISTURE  0   // PC1 - 降雨量

// DMA 搬运的总通道数
#define ADC_CHANNEL_COUNT        2

// 声明全局 volatile 数组，防止被编译器缓存优化
extern volatile uint16_t adc_buffer[ADC_CHANNEL_COUNT];

#endif // GLOBAL_ADC_BUFFER_H