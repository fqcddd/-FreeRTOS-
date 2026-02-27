/**
* @file BleTask.c
 * @brief 蓝牙通信任务
 *
 * 本任务负责：
 * 1. 从BLE队列接收报警消息
 * 2. 通过UART3（DMA方式）发送消息到蓝牙模块
 * 3. 等待发送完成后释放消息内存
 *
 * 任务优先级：osPriorityLow（低优先级，不影响实时性要求高的任务）
 * 任务阻塞：使用osWaitForever等待队列消息，有消息时才执行
 *
 * 消息格式：
 * JSON格式的字符串，例如：{"type":"warning", "reason":"temperature_high", "value":35.5}
 *
 * @note
 * - 消息由SensorTask通过pvPortMalloc分配内存创建
 * - 本任务负责在发送完成后使用vPortFree释放内存
 * - 使用DMA方式发送，提高效率
 */

#include "cmsis_os2.h"
#include "main.h"
#include "stm32f1xx_hal_uart.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>
#include "freertos.h"
#include "farmState.h"

/**
 * @brief 蓝牙通信任务主函数
 *
 * 任务执行流程：
 * 1. 从BLE队列阻塞等待消息（队列为空时任务挂起）
 * 2. 收到消息后，通过UART3（DMA方式）发送到蓝牙模块
 * 3. 等待DMA发送完成（轮询UART状态）
 * 4. 释放消息内存（使用vPortFree）
 * 5. 继续等待下一条消息
 *
 * @param argument 任务参数（未使用）
 *
 * @note
 * - 使用osWaitForever阻塞等待，队列为空时任务挂起，不占用CPU
 * - 消息指针存储在队列中，实际消息内容在FreeRTOS堆中
 * - 使用DMA发送可以提高效率，避免阻塞CPU
 * - 必须等待发送完成后再释放内存，否则可能导致数据损坏
 */
void StartBLETask(void *argument) {
    // 主循环：持续处理队列中的消息
    for (;;) {
        char *msg;

        // 从BLE队列阻塞等待消息（队列为空时任务挂起，等待时间无限）
        osMessageQueueGet(BLEQueueHandle, &msg, NULL, osWaitForever);

        if (msg != NULL) {
             // 通过UART2使用DMA方式发送消息到蓝牙模块
             HAL_UART_Transmit_DMA(&huart2, (uint8_t *)msg, strlen(msg));

             // 等待DMA发送完成（轮询UART状态，直到发送完成）
             while (1) {
                 if (HAL_UART_GetState(&huart2) == HAL_UART_STATE_READY) {
                     break; // UART状态为READY表示发送完成
                 }
                 osDelay(1); // 延时1ms后继续检查
             }

            // 发送完成后，释放消息内存（消息由SensorTask使用pvPortMalloc分配）
            vPortFree(msg);
            // 【核心改造】：活干完了，待办任务 -1
            if (ble_pending_msgs > 0) {
                ble_pending_msgs--;
            }
        }
    }
}