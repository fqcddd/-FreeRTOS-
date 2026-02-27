#include "knob.h"
#include "stm32f1xx_hal_tim.h"
#include "tim.h" // 包含 htim4 的定义

#define COUNTER_INIT_VALUE (65535/2)

void Knot_SetCounter(uint32_t count) {
    __HAL_TIM_SET_COUNTER(&htim4, count);
}

uint32_t Knob_GetCounter() {
    return __HAL_TIM_GET_COUNTER(&htim4);
}

void Knob_Init() {
    HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
    Knot_SetCounter(COUNTER_INIT_VALUE);
}

KnobDirection Knob_IsRotating() {
    uint32_t counter = Knob_GetCounter();
    KnobDirection direction = KNOB_DIR_NONE;
    if (counter > COUNTER_INIT_VALUE) {
        direction = KNOB_DIR_RIGHT;
    } else if (counter < COUNTER_INIT_VALUE) {
        direction = KNOB_DIR_LEFT;
    }
    Knot_SetCounter(COUNTER_INIT_VALUE);
    return direction;
}