/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define SQW_Pin GPIO_PIN_0
#define SQW_GPIO_Port GPIOD
#define SQW_EXTI_IRQn EXTI0_IRQn
#define SEG_A_Pin GPIO_PIN_0
#define SEG_A_GPIO_Port GPIOA
#define SEG_B_Pin GPIO_PIN_1
#define SEG_B_GPIO_Port GPIOA
#define SEG_C_Pin GPIO_PIN_2
#define SEG_C_GPIO_Port GPIOA
#define SEG_D_Pin GPIO_PIN_3
#define SEG_D_GPIO_Port GPIOA
#define SEG_E_Pin GPIO_PIN_4
#define SEG_E_GPIO_Port GPIOA
#define SEG_F_Pin GPIO_PIN_5
#define SEG_F_GPIO_Port GPIOA
#define SEG_G_Pin GPIO_PIN_6
#define SEG_G_GPIO_Port GPIOA
#define SEG_H_Pin GPIO_PIN_7
#define SEG_H_GPIO_Port GPIOA
#define L1_Pin GPIO_PIN_0
#define L1_GPIO_Port GPIOB
#define L2_Pin GPIO_PIN_1
#define L2_GPIO_Port GPIOB
#define L3_Pin GPIO_PIN_2
#define L3_GPIO_Port GPIOB
#define BUTTON_SELECT_Pin GPIO_PIN_13
#define BUTTON_SELECT_GPIO_Port GPIOB
#define BUTTON_SELECT_EXTI_IRQn EXTI15_10_IRQn
#define BUTTON_PLUS_Pin GPIO_PIN_14
#define BUTTON_PLUS_GPIO_Port GPIOB
#define BUTTON_PLUS_EXTI_IRQn EXTI15_10_IRQn
#define BUTTON_MINUS_Pin GPIO_PIN_15
#define BUTTON_MINUS_GPIO_Port GPIOB
#define BUTTON_MINUS_EXTI_IRQn EXTI15_10_IRQn
#define BUZZER_Pin GPIO_PIN_8
#define BUZZER_GPIO_Port GPIOA
#define VFD_AC_Pin GPIO_PIN_15
#define VFD_AC_GPIO_Port GPIOA
#define L4_Pin GPIO_PIN_3
#define L4_GPIO_Port GPIOB
#define L5_Pin GPIO_PIN_4
#define L5_GPIO_Port GPIOB
#define L6_Pin GPIO_PIN_5
#define L6_GPIO_Port GPIOB
#define L7_Pin GPIO_PIN_6
#define L7_GPIO_Port GPIOB
#define L8_Pin GPIO_PIN_7
#define L8_GPIO_Port GPIOB
#define DHT_interface_Pin GPIO_PIN_8
#define DHT_interface_GPIO_Port GPIOB
#define OneWire_Pin GPIO_PIN_9
#define OneWire_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
