#include "pump.h"

/**
 * @brief 启动水泵
 */
void Pump_On(void){
    HAL_GPIO_WritePin(Pump_GPIO_Port, Pump_Pin, GPIO_PIN_SET);
}

/**
 * @brief 停止水泵
 */
void Pump_Off(void){
    HAL_GPIO_WritePin(Pump_GPIO_Port, Pump_Pin, GPIO_PIN_RESET);
}