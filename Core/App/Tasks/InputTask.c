/**
 * @file InputTask.c
 * @brief 用户输入处理任务
 *
 * 本任务负责：
 * 1. 检测按键输入（KEY1、KEY3）
 * 2. 检测旋钮旋转方向
 * 3. 根据输入控制页面切换和阈值编辑
 *
 * 任务优先级：osPriorityHigh（高优先级，保证用户输入响应及时）
 * 任务周期：10ms（快速响应）
 *
 * 用户交互逻辑：
 * - KEY1：切换页面（首页 <-> 阈值设置页）
 * - KEY3：在阈值设置页中，切换浏览/编辑模式
 * - 旋钮左旋/右旋：
 *   - 浏览模式：切换选中的阈值项
 *   - 编辑模式：减小/增大当前选中项的值
 */

#include "cmsis_os2.h"
#include "global/farmState.h"
#include "global/screen.h"
#include "key.h"
#include "knob.h"
#include <stdint.h>

/**
 * @brief 编辑阈值数值
 *
 * 根据编辑索引和方向修改对应的阈值参数
 * 修改时会进行边界检查，确保：
 * - 最小值不超过最大值
 * - 最大值不小于最小值
 * - 数值在合理范围内
 *
 * @param index 要编辑的阈值项索引（RangeEditIndex枚举值）
 * @param direction 修改方向：-1表示减小，1表示增大
 *
 * @note
 * - 浮点数类型（温度、湿度）每次调整0.1
 * - 整数类型（土壤湿度、光照、降雨量）每次调整1
 */
void EditRangeValue(RangeEditIndex index, int8_t direction) {
  switch (index) {
  case RANGE_EDIT_TEMPERATURE_MIN:
    // 最小温度：每次调整0.1℃，且不能超过最大温度
    if (farmSafeRange.minTemperature + 0.1 * direction < farmSafeRange.maxTemperature) {
      farmSafeRange.minTemperature += 0.1 * direction;
    }
    break;

  case RANGE_EDIT_TEMPERATURE_MAX:
    // 最大温度：每次调整0.1℃，且不能低于最小温度
    if (farmSafeRange.maxTemperature + 0.1 * direction > farmSafeRange.minTemperature) {
      farmSafeRange.maxTemperature += 0.1 * direction;
    }
    break;

  case RANGE_EDIT_HUMIDITY_MIN:
    // 最小湿度：每次调整0.1%，且不能超过最大湿度
    if (farmSafeRange.minHumidity + 0.1 * direction < farmSafeRange.maxHumidity) {
      farmSafeRange.minHumidity += 0.1 * direction;
    }
    break;

  case RANGE_EDIT_HUMIDITY_MAX:
    // 最大湿度：每次调整0.1%，且不能低于最小湿度
    if (farmSafeRange.maxHumidity + 0.1 * direction > farmSafeRange.minHumidity) {
      farmSafeRange.maxHumidity += 0.1 * direction;
    }
    break;

  case RANGE_EDIT_RAIN_GAUGE_MAX:
    // 最大降雨量：每次调整1%，范围0-100%
    if (farmSafeRange.maxRainGauge + direction > 0 && farmSafeRange.maxRainGauge + direction < 100) {
      farmSafeRange.maxRainGauge += direction;
    }
    break;

  case RANGE_EDIT_SOIL_MOISTURE_MIN:
    // 最小土壤湿度：每次调整1%，范围0-100%，且不能超过最大土壤湿度
    if (farmSafeRange.minSoilMoisture + direction < farmSafeRange.maxSoilMoisture
      && farmSafeRange.minSoilMoisture + direction > 0
    ) {
      farmSafeRange.minSoilMoisture += direction;
    }
    break;

  case RANGE_EDIT_SOIL_MOISTURE_MAX:
    // 最大土壤湿度：每次调整1%，范围0-100%，且不能低于最小土壤湿度
    if (farmSafeRange.maxSoilMoisture + direction > farmSafeRange.minSoilMoisture
      && farmSafeRange.maxSoilMoisture + direction < 100
    ) {
      farmSafeRange.maxSoilMoisture += direction;
    }
    break;

  case RANGE_EDIT_LIGHT_INTENSITY_MIN:
    // 最小光照强度：每次调整1 lx，范围0-10000 lx，且不能超过最大光照强度
    if (farmSafeRange.minLightIntensity + direction < farmSafeRange.maxLightIntensity
      && farmSafeRange.minLightIntensity + direction > 0
    ) {
      farmSafeRange.minLightIntensity += direction;
    }
    break;

  case RANGE_EDIT_LIGHT_INTENSITY_MAX:
    // 最大光照强度：每次调整1 lx，范围0-10000 lx，且不能低于最小光照强度
    if (farmSafeRange.maxLightIntensity + direction > farmSafeRange.minLightIntensity
      && farmSafeRange.maxLightIntensity + direction < 10000
    ) {
      farmSafeRange.maxLightIntensity += direction;
    }
    break;

  default:
    break;
  }
}

/**
 * @brief 输入任务主函数
 *
 * 任务执行流程：
 * 1. 初始化旋钮（按键通过GPIO中断处理，无需初始化）
 * 2. 进入主循环：
 *    - 检测KEY1按下：切换页面
 *    - 在阈值设置页时：
 *      * 检测KEY3按下：切换浏览/编辑模式
 *      * 检测旋钮旋转：
 *        - 浏览模式：切换选中的阈值项
 *        - 编辑模式：修改当前选中项的值
 *    - 延时10ms后继续下一次检测
 *
 * @param argument 任务参数（未使用）
 *
 * @note
 * - 任务优先级较高，保证用户输入响应及时
 * - 10ms的检测周期既能保证响应速度，又不会占用过多CPU资源
 */
void StartInputTask(void *argument) {
  // 初始化旋钮（用于检测旋转方向）
  Knob_Init();

  // 主循环：持续检测用户输入
  for (;;) {

    // 【核心动态门卫】：
    // 如果当前在阈值设置页 (PAGE_RANGE)，我们需要随时读取旋钮，最多睡 50ms。
    // 如果在其他页面，旋钮无效，直接进入 osWaitForever 深度死睡，直到 EXTI 中断把它踹醒！
    uint32_t waitTime = (pageIndex == PAGE_RANGE) ? 50 : osWaitForever;

    // 任务在这里挂起等待：
    // 1. 要么被按键的 EXTI 中断（信号量）瞬间叫醒
    // 2. 要么 50ms 超时自然醒（仅限设置页）
    osSemaphoreAcquire(InputEventSemHandle, waitTime);

    // // 检测KEY1按下：切换页面（首页 <-> 阈值设置页）
    // if (isKey1Clicked()) {
    //   ScreenPage_NextPage();
    // }

    // ==========================================
    // 1. 直接处理 KEY1 (PE4)
    // ==========================================
    if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == GPIO_PIN_RESET) {
      osDelay(20); // 二次软件消抖，确保万无一失
      if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == GPIO_PIN_RESET) {

        ScreenPage_NextPage(); // 执行翻页

        // 【核心防连按机制】：死等用户松开手指！
        // 只要引脚还是低电平，就在这里转圈，并且交出 CPU 避免卡死其他任务
        while(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) == GPIO_PIN_RESET) {
          osDelay(10);
        }
      }
    }

    // // 在阈值设置页时，处理阈值编辑相关输入
    // if (pageIndex == PAGE_RANGE) {
    //   // 检测KEY3按下：切换浏览/编辑模式
    //   if (isKey3Clicked()) {
    //     RangeEditState_Toggle();
    //   }

    // ==========================================
    // 2. 只有在设置页时，才处理 KEY3 和旋钮
    // ==========================================
    if (pageIndex == PAGE_RANGE) {

      // 直接处理 KEY3 (PE3)
      if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == GPIO_PIN_RESET) {
        osDelay(20);
        if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == GPIO_PIN_RESET) {

          RangeEditState_Toggle(); // 切换编辑模式

          // 死等用户松手
          while(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == GPIO_PIN_RESET) {
            osDelay(10);
          }
        }
      }

      // 检测旋钮旋转方向
      KnobDirection direction = Knob_IsRotating();

      if (rangeEditState == RANGE_EDIT_STATE_NORMAL) {
        // 浏览模式：通过旋钮切换选中的阈值项
        if (direction == KNOB_DIR_LEFT) {
          RangeEditIndex_Prev(); // 左旋：切换到上一个阈值项
        } else if (direction == KNOB_DIR_RIGHT) {
          RangeEditIndex_Next(); // 右旋：切换到下一个阈值项
        }
      } else if (rangeEditState == RANGE_EDIT_STATE_EDITING) {
        // 编辑模式：通过旋钮修改当前选中阈值项的值
        if (direction == KNOB_DIR_LEFT) {
          EditRangeValue(rangeEditIndex, -1); // 左旋：减小值
        } else if (direction == KNOB_DIR_RIGHT) {
          EditRangeValue(rangeEditIndex, 1);  // 右旋：增大值
        }
      }
    }

    // 延时10ms后继续下一次输入检测
    osDelay(25);
  }
}
