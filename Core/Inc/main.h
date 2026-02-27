/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "cmsis_os2.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
// 消息队列句柄 用于其他任务向BLE任务发送信息
extern osMessageQueueId_t BLEQueueHandle;
// I2C1 互斥锁句柄 防止AHT20与OLED同时访问I2C总线
extern osMutexId_t i2c2MutexHandle;
// 蜂鸣器定时器句柄
extern osTimerId_t BeepTimerHandle;
//按键中断操作句柄
extern osSemaphoreId_t InputEventSemHandle;
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void SystemClock_Config(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define KEY3_Pin GPIO_PIN_3
#define KEY3_GPIO_Port GPIOE
#define KEY3_EXTI_IRQn EXTI3_IRQn
#define KEY1_Pin GPIO_PIN_4
#define KEY1_GPIO_Port GPIOE
#define KEY1_EXTI_IRQn EXTI4_IRQn
#define SDA_Pin GPIO_PIN_4
#define SDA_GPIO_Port GPIOC
#define SCL_Pin GPIO_PIN_5
#define SCL_GPIO_Port GPIOC
#define Pump_Pin GPIO_PIN_13
#define Pump_GPIO_Port GPIOG
#define LED_Pin GPIO_PIN_5
#define LED_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
