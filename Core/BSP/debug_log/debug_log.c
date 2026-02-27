#include "debug_log.h"
#include "main.h"      // 引入 HAL 库的基础定义
#include "cmsis_os.h"  // 引入 FreeRTOS 的 osDelay
#include <stdio.h>
#include <stdarg.h>

// 【极其致命的一步】：告诉编译器，huart1 这个硬件句柄在 main.c 里已经初始化好了，直接拿来用！
extern UART_HandleTypeDef huart1;

// 定义专属的 DMA 发送缓冲区（对外部隐藏，极其安全）
#define DMA_PRINTF_BUF_SIZE 256
char dma_printf_buffer[DMA_PRINTF_BUF_SIZE];

// ==========================================
// 非阻塞式 DMA 串口打印函数
// ==========================================
void DMA_Printf(const char *format, ...)
{
    // 1. 防撞车机制：等待上一次发送完成
    while (huart1.gState != HAL_UART_STATE_READY) {
        osDelay(1); 
    }

    // 2. 准备组装字符串
    va_list args;
    va_start(args, format);

    // 3. 将格式化后的数据写入专属缓冲区
    uint16_t len = vsnprintf(dma_printf_buffer, DMA_PRINTF_BUF_SIZE, format, args);
    va_end(args);

    // 4. 呼叫 DMA 开始搬运
    HAL_UART_Transmit_DMA(&huart1, (uint8_t*)dma_printf_buffer, len);
}

// ==========================================
// 顺手把标准 printf 重定向也搬过来，保持 main.c 清爽
// ==========================================
#ifdef __GNUC__
int _write(int file, char *ptr, int len) {
    HAL_UART_Transmit(&huart1, (uint8_t *)ptr, len, HAL_MAX_DELAY);
    return len;
}
#else
int fputc(int ch, FILE *f) {
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}
#endif