/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum {
    ROJO_A,
    AMARILLO_A,
    VERDE_A,
    VERDE_B,
    AMARILLO_B,
    ROJO_B,
    TOTAL_SECUENCIA
} Secuencia;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define rojoA GPIO_PIN_8
#define verdeA GPIO_PIN_9
#define amarilloA GPIO_PIN_11
#define verdeB GPIO_PIN_12
#define rojoB GPIO_PIN_13
#define amarilloB GPIO_PIN_15
#define EncenderTodos rojoA|verdeA|amarilloA|verdeB|rojoB|amarilloB
#define ApagarTodos (rojoA|verdeA|amarilloA|verdeB|rojoB|amarilloB) << 16
#define leds_GPIO_Port GPIOD
#define TENCENDIDO 2000
#define TAPAGADO   1000
#define TREBOTE    150
#define ESTADOiNICIAL ROJO_A
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
volatile uint8_t  solicitud     = 0;
volatile uint8_t  filtrado      = 0;
volatile uint32_t tiempo_inicio = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
void     inicio(void);
void     apagarTodos(void);
void     encenderTodos(void);
void     apagar(Secuencia estadoActual);
void     encender(Secuencia estadoActual);
Secuencia avanzar(Secuencia estadoActual);
void     filtrar(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
  Secuencia estadoActual = ESTADOiNICIAL;
  inicio();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if (1 == filtrado)
	      filtrar();

	  if (1 == solicitud) {
	      estadoActual = avanzar(estadoActual);
	      solicitud = 0;
	  }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 50;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, rojoA_Pin|verdeA_Pin|amarilloA_Pin|verdeB_Pin
                          |rojoB_Pin|amarilloB_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : boton_Pin */
  GPIO_InitStruct.Pin = boton_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(boton_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : rojoA_Pin verdeA_Pin amarilloA_Pin verdeB_Pin
                           rojoB_Pin amarilloB_Pin */
  GPIO_InitStruct.Pin = rojoA_Pin|verdeA_Pin|amarilloA_Pin|verdeB_Pin
                          |rojoB_Pin|amarilloB_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void inicio(void) {
    encenderTodos();
    HAL_Delay(TENCENDIDO);
    apagarTodos();
    HAL_Delay(TAPAGADO);
    encender(ESTADOiNICIAL);
}

void apagarTodos(void) {
    leds_GPIO_Port->BSRR = ApagarTodos;
}

void encenderTodos(void) {
    leds_GPIO_Port->BSRR = EncenderTodos;
}

void apagar(Secuencia estadoActual) {
    switch (estadoActual) {
        case ROJO_A:     HAL_GPIO_WritePin(leds_GPIO_Port, rojoA_Pin,     GPIO_PIN_RESET); break;
        case AMARILLO_A: HAL_GPIO_WritePin(leds_GPIO_Port, amarilloA_Pin, GPIO_PIN_RESET); break;
        case VERDE_A:    HAL_GPIO_WritePin(leds_GPIO_Port, verdeA_Pin,    GPIO_PIN_RESET); break;
        case VERDE_B:    HAL_GPIO_WritePin(leds_GPIO_Port, verdeB_Pin,    GPIO_PIN_RESET); break;
        case AMARILLO_B: HAL_GPIO_WritePin(leds_GPIO_Port, amarilloB_Pin, GPIO_PIN_RESET); break;
        case ROJO_B:     HAL_GPIO_WritePin(leds_GPIO_Port, rojoB_Pin,     GPIO_PIN_RESET); break;
        default:         apagarTodos(); break;
    }
}

void encender(Secuencia estadoActual) {
    switch (estadoActual) {
        case ROJO_A:     HAL_GPIO_WritePin(leds_GPIO_Port, rojoA_Pin,     GPIO_PIN_SET); break;
        case AMARILLO_A: HAL_GPIO_WritePin(leds_GPIO_Port, amarilloA_Pin, GPIO_PIN_SET); break;
        case VERDE_A:    HAL_GPIO_WritePin(leds_GPIO_Port, verdeA_Pin,    GPIO_PIN_SET); break;
        case VERDE_B:    HAL_GPIO_WritePin(leds_GPIO_Port, verdeB_Pin,    GPIO_PIN_SET); break;
        case AMARILLO_B: HAL_GPIO_WritePin(leds_GPIO_Port, amarilloB_Pin, GPIO_PIN_SET); break;
        case ROJO_B:     HAL_GPIO_WritePin(leds_GPIO_Port, rojoB_Pin,     GPIO_PIN_SET); break;
        default:         encenderTodos(); break;
    }
}

Secuencia avanzar(Secuencia estadoActual) {
    apagar(estadoActual);
    Secuencia estadoSiguiente = (Secuencia)((estadoActual + 1) % TOTAL_SECUENCIA);
    encender(estadoSiguiente);
    return estadoSiguiente;
}

void filtrar(void) {
    uint32_t tiempo_actual = HAL_GetTick();
    if (TREBOTE <= (tiempo_actual - tiempo_inicio)) {
        GPIO_PinState estado_boton = HAL_GPIO_ReadPin(boton_GPIO_Port, boton_Pin);
        if (GPIO_PIN_RESET == estado_boton)
            solicitud = 1;
        filtrado = 0;
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == boton_Pin && 0 == filtrado) {
        tiempo_inicio = HAL_GetTick();
        filtrado = 1;
    }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
