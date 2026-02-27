/**
* @file adc_buffer.c
 * @brief 全局 ADC DMA 数据缓冲区定义
 */
#include "adc_buffer.h"


// 真正的内存分配，初始化为 0
volatile uint16_t adc_buffer[ADC_CHANNEL_COUNT] = {0};

