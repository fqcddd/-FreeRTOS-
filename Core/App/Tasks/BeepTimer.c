/**
* @file BeepTimer.c
 * @brief 蜂鸣器软件定时器控制模块
 *
 * 本模块提供蜂鸣器控制功能：
 * - Beep_on()：启动蜂鸣器（周期性响铃）
 * - Beep_off()：停止蜂鸣器
 * - BeepTimerCallback()：定时器回调函数，实现蜂鸣器的开关切换
 *
 * 工作原理：
 * - 使用FreeRTOS软件定时器，周期为500ms
 * - 定时器回调函数在每次触发时切换蜂鸣器状态（开/关）
 * - 实现周期性响铃效果（500ms开，500ms关）
 *
 * @note 使用TIM4的PWM通道4控制蜂鸣器
 */

#include "cmsis_os2.h"
#include "main.h"
#include "tim.h"

/**
 * @brief 启动蜂鸣器
 *
 * 启动软件定时器，定时器会周期性触发回调函数，实现蜂鸣器的周期性响铃
 * 定时器周期为500ms，即每500ms切换一次蜂鸣器状态
 *
 * @note 由SensorTask在检测到报警时调用
 */
void Beep_on(void) {
    // 启动软件定时器，周期500ms（周期性定时器）
    osTimerStart(BeepTimerHandle, 500);
}

/**
 * @brief 停止蜂鸣器
 *
 * 停止软件定时器，并立即关闭蜂鸣器PWM输出
 *
 * @note 由SensorTask在无报警时调用
 */
void Beep_off(void) {
    // 停止软件定时器
    osTimerStop(BeepTimerHandle);

    // 停止PWM输出并设置占空比为0
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
}

/**
 * @brief 蜂鸣器定时器回调函数
 *
 * 软件定时器触发时调用此函数，实现蜂鸣器状态的周期性切换
 * 每次触发时，在开启和关闭状态之间切换，实现周期性响铃效果
 *
 * 工作流程：
 * - 状态0：开启PWM，设置占空比（产生声音）
 * - 状态1：关闭PWM，设置占空比为0（停止声音）
 * - 下次触发时切换状态
 *
 * @param argument 定时器参数（未使用）
 *
 * @note
 * - 使用静态变量beep_state保持状态（在函数调用之间保持）
 * - 占空比设置为100，可根据需要调整音量
 * - 定时器周期为500ms，实现500ms开、500ms关的周期性响铃
 */
void BeepTimerCallback(void *argument){
    static uint8_t beep_state = 0;  // 蜂鸣器状态：0-关闭，1-开启（静态变量保持状态）

    if(beep_state == 0){
        // 状态0：开启蜂鸣器
        HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);  // 启动PWM输出
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 250);  // 设置占空比（音量）
        beep_state = 1;  // 切换到开启状态
    }else{
        // 状态1：关闭蜂鸣器
        HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);  // 停止PWM输出
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);  // 设置占空比为0
        beep_state = 0;  // 切换到关闭状态
    }
}