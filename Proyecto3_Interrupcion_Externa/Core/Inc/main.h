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
#include "stm32f4xx_hal.h"

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
#define boton_Pin GPIO_PIN_5
#define boton_GPIO_Port GPIOA
#define boton_EXTI_IRQn EXTI9_5_IRQn
#define rojoA_Pin GPIO_PIN_8
#define rojoA_GPIO_Port GPIOD
#define verdeA_Pin GPIO_PIN_9
#define verdeA_GPIO_Port GPIOD
#define amarilloA_Pin GPIO_PIN_11
#define amarilloA_GPIO_Port GPIOD
#define verdeB_Pin GPIO_PIN_12
#define verdeB_GPIO_Port GPIOD
#define rojoB_Pin GPIO_PIN_13
#define rojoB_GPIO_Port GPIOD
#define amarilloB_Pin GPIO_PIN_15
#define amarilloB_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
