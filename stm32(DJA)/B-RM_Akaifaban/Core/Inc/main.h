/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
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
#define LASTER_Pin GPIO_PIN_13
#define LASTER_GPIO_Port GPIOG
#define LIMIT_AW_Pin GPIO_PIN_7
#define LIMIT_AW_GPIO_Port GPIOI
#define LIMIT_DS_Pin GPIO_PIN_6
#define LIMIT_DS_GPIO_Port GPIOI
#define LIMIT_AS_Pin GPIO_PIN_5
#define LIMIT_AS_GPIO_Port GPIOI
#define LIMIT_DW_Pin GPIO_PIN_2
#define LIMIT_DW_GPIO_Port GPIOI
#define POWER1_CTRL_Pin GPIO_PIN_2
#define POWER1_CTRL_GPIO_Port GPIOH
#define POWER2_CTRL_Pin GPIO_PIN_3
#define POWER2_CTRL_GPIO_Port GPIOH
#define POWER3_CTRL_Pin GPIO_PIN_4
#define POWER3_CTRL_GPIO_Port GPIOH
#define LED_8_Pin GPIO_PIN_8
#define LED_8_GPIO_Port GPIOG
#define POWER4_CTRL_Pin GPIO_PIN_5
#define POWER4_CTRL_GPIO_Port GPIOH
#define LED_7_Pin GPIO_PIN_7
#define LED_7_GPIO_Port GPIOG
#define LED_6_Pin GPIO_PIN_6
#define LED_6_GPIO_Port GPIOG
#define MPU6500_NSS_Pin GPIO_PIN_6
#define MPU6500_NSS_GPIO_Port GPIOF
#define LED_5_Pin GPIO_PIN_5
#define LED_5_GPIO_Port GPIOG
#define LED_4_Pin GPIO_PIN_4
#define LED_4_GPIO_Port GPIOG
#define LED_3_Pin GPIO_PIN_3
#define LED_3_GPIO_Port GPIOG
#define LED_2_Pin GPIO_PIN_2
#define LED_2_GPIO_Port GPIOG
#define KEY_Pin GPIO_PIN_2
#define KEY_GPIO_Port GPIOB
#define LED_1_Pin GPIO_PIN_1
#define LED_1_GPIO_Port GPIOG
#define FAN_EN_Pin GPIO_PIN_13
#define FAN_EN_GPIO_Port GPIOD
#define LED_RED_Pin GPIO_PIN_11
#define LED_RED_GPIO_Port GPIOE
#define LED_GREEN_Pin GPIO_PIN_14
#define LED_GREEN_GPIO_Port GPIOF
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
