

#include "key.h"
#include "cmsis_os2.h"

#define IS_KEY1_PRESSED() (HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == GPIO_PIN_RESET)
#define IS_KEY3_PRESSED() (HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin) == GPIO_PIN_RESET)

uint8_t isKey1Clicked() {
    static uint8_t pressed = 0; // 用于每次按键只返回一次点击事件
    if (IS_KEY1_PRESSED() && !pressed) {
        delay(10); // 消抖延时
        if (IS_KEY1_PRESSED()) {
            pressed = 1;
            return 1; // 按键被点击
        }
    }
    if (pressed && !IS_KEY1_PRESSED()) {
        pressed = 0; // 按键释放，重置状态
    }
    return 0;
}

uint8_t isKey3Clicked() {
    static uint8_t pressed = 0;
    if (IS_KEY3_PRESSED() && !pressed) {
        delay(10); // 消抖延时
        if (IS_KEY3_PRESSED()) {
            pressed = 1;
            return 1; // 按键被点击
        }
    }
    if (pressed && !IS_KEY3_PRESSED()) {
        pressed = 0; // 按键释放，重置状态
    }
    return 0;
}