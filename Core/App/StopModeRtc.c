#include "StopModeRtc.h"
#include "rtc.h" // 确保引入了 RTC 硬件句柄 hrtc
#include "main.h"
#include "cmsis_os.h"


//extern void SystemClock_Config(void);
// ==========================================
// 设定 RTC 闹钟并进入 Stop 模式的终极函数
// 参数 seconds: 你想深度睡眠多少秒
// ==========================================
void Enter_Deep_Stop_Mode_With_RTC(uint32_t seconds) 
{

    // 1. 获取当前的 RTC 时间
    RTC_TimeTypeDef sTime = {0};
    RTC_DateTypeDef sDate = {0}; // F1 虽然没有硬件日期，但 HAL 库流程需要读取清除标志
    HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

    // 2. 计算唤醒时间 (简单粗暴的秒数累加)
    // 注意：如果是生产级代码，这里需要处理 60秒进位 到分钟、小时的复杂逻辑
    // 为了快速验证，STM32F1 允许我们直接操作底层的 32 位计数器更方便：
    uint32_t current_counter = hrtc.Instance->CNTL | (hrtc.Instance->CNTH << 16);
    uint32_t alarm_counter = current_counter + seconds;

    HAL_PWR_EnableBkUpAccess(); // 允许访问备份域

    // ==========================================
    // STM32F1 专属：暴力写入 32 位闹钟寄存器
    // ==========================================

    // 1. 等待上一次 RTC 寄存器写操作完成 (极度重要，防止死机)
    while((hrtc.Instance->CRL & RTC_CRL_RTOFF) == (uint32_t)RESET) {}

    // 2. 强行进入 RTC 配置模式 (设置 CNF 位)
    hrtc.Instance->CRL |= RTC_CRL_CNF;

    // 3. 将计算好的未来秒数，分别写入闹钟高 16 位和低 16 位寄存器
    hrtc.Instance->ALRH = alarm_counter >> 16;
    hrtc.Instance->ALRL = alarm_counter & 0xFFFF;

    // 4. 退出配置模式 (清除 CNF 位)
    hrtc.Instance->CRL &= ~RTC_CRL_CNF;

    // 5. 再次等待写操作物理生效
    while((hrtc.Instance->CRL & RTC_CRL_RTOFF) == (uint32_t)RESET) {}
    
    // 3. 使能 RTC 闹钟中断 (EXTI Line 17)
    hrtc.Instance->CRH |= RTC_CRH_ALRIE; // F1 专属：暴力开启 RTC 闹钟中断允许位
    __HAL_RTC_ALARM_EXTI_ENABLE_IT();
    __HAL_RTC_ALARM_EXTI_ENABLE_RISING_EDGE();

    // 4. 睡前准备：关闭无关外设，暂停 SysTick
    // OLED_Display_Off(); 
    HAL_SuspendTick(); 
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET); // 亮红灯：代表彻底死睡了

    // 5. 拔掉主时钟电源，进入深寒 (Stop 模式)！
    HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);

    // ==========================================
    // 漫长的等待... 此时整板电流低至微安级
    // 闹钟时间到 -> 触发中断 -> 瞬间苏醒！
    // ==========================================

    // 6. 醒来第一件事：重新启动 72MHz 高速心脏！
    SystemClock_Config();

    // 7. 恢复滴答定时器
    HAL_ResumeTick();
    
    // 8. 唤醒外设
    // OLED_Display_On();
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET); // 灭红灯：代表醒来干活了
    
    // 清除闹钟中断标志，为下一次睡眠做准备
    __HAL_RTC_ALARM_EXTI_CLEAR_FLAG();
    hrtc.Instance->CRL &= ~RTC_CRL_ALRF; // F1 专属：暴力清除 RTC 闹钟触发标志位
}