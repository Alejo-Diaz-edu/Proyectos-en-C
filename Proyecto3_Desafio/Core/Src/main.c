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
typedef enum { ROJO_A, AMARILLO_A, VERDE_A, TOTAL_SECUENCIAA} SecuenciaA;

typedef enum { ROJO_B, AMARILLO_B, VERDE_B, TOTAL_SECUENCIAB} SecuenciaB;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BOTONES_GPIO_PORT GPIOA //el buzzer está en esta categoria
#define LUCES_GPIO_PORT GPIOD

#define encenderTodos ROJOA_Pin|VERDEA_Pin|AMARILLOA_Pin|VERDEB_Pin|ROJOB_Pin|AMARILLOB_Pin
#define apagarTodos (ROJOA_Pin|VERDEA_Pin|AMARILLOA_Pin|VERDEB_Pin|ROJOB_Pin|AMARILLOB_Pin) << 16

#define T2S 2000
#define T1S 1000
#define TREBOTE 150

#define ESTADOINICIALA ROJO_A
#define ESTADOINICIALB ROJO_B
//#define SecuenciaA estadoActualA
//#define SecuenciaB estadoActualB

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
volatile uint8_t solicitud = 0;
volatile uint32_t lastDebounceTickSw1 = 0;
volatile uint32_t lastDebounceTickSw2 = 0;
volatile uint32_t lastDebounceTickSw3 = 0;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
void inicio();
void Funcionapagartodos();
void Funcionencendertodos();
void apagarA(SecuenciaA estadoActualA);
void apagarB(SecuenciaB estadoActualB);
void encenderA(SecuenciaA estadoActualA);
void encenderB(SecuenciaB estadoActualB);
SecuenciaA avanzarA(SecuenciaA estadoActualA);
SecuenciaB avanzarB(SecuenciaB estadoActualB);


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
	SecuenciaA estadoActualA = ESTADOINICIALA;
	SecuenciaB estadoActualB = ESTADOINICIALB;
	int contador = 0;

	inicio();
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		if (solicitud == 1) {
			estadoActualA = avanzarA(estadoActualA);
			solicitud = 0;
		}
		if (solicitud == 2) {
			estadoActualB = avanzarB(estadoActualB);
			solicitud = 0;
		}
		if (solicitud == 3) {
			contador++;
			solicitud = 0;
			if (contador == 5) {
				HAL_GPIO_WritePin(BOTONES_GPIO_PORT, BUZZ_Pin, GPIO_PIN_SET);
				inicio();
				HAL_Delay(5000);
				HAL_GPIO_WritePin(BOTONES_GPIO_PORT, BUZZ_Pin, GPIO_PIN_RESET);
				
				contador = 0;
			}
		}
	}
}
/* USER CODE END 3 */


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
	HAL_GPIO_WritePin(BUZZ_GPIO_Port, BUZZ_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOD, ROJOA_Pin|VERDEA_Pin|AMARILLOA_Pin|VERDEB_Pin
			|ROJOB_Pin|AMARILLOB_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin : SW1_Pin */
	GPIO_InitStruct.Pin = SW1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(SW1_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : BUZZ_Pin */
	GPIO_InitStruct.Pin = BUZZ_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(BUZZ_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : SW2_Pin */
	GPIO_InitStruct.Pin = SW2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(SW2_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : SW3_Pin */
	GPIO_InitStruct.Pin = SW3_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(SW3_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : ROJOA_Pin VERDEA_Pin AMARILLOA_Pin VERDEB_Pin
                           ROJOB_Pin AMARILLOB_Pin */
	GPIO_InitStruct.Pin = ROJOA_Pin|VERDEA_Pin|AMARILLOA_Pin|VERDEB_Pin
			|ROJOB_Pin|AMARILLOB_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI2_IRQn);

	HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI4_IRQn);

	HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

	/* USER CODE BEGIN MX_GPIO_Init_2 */

	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void inicio(){

	Funcionencendertodos();
	HAL_Delay(T2S);
	Funcionapagartodos();
	HAL_Delay(T1S);
	HAL_GPIO_WritePin(LUCES_GPIO_PORT, ROJOA_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LUCES_GPIO_PORT, ROJOB_Pin, GPIO_PIN_SET);
}

void Funcionencendertodos(void) {
	LUCES_GPIO_PORT->BSRR=encenderTodos;
}

void Funcionapagartodos(void) {
	LUCES_GPIO_PORT->BSRR=apagarTodos;
}

void apagarA(SecuenciaA estadoActualA) {
	switch(estadoActualA){
	case ROJO_A:
		HAL_GPIO_WritePin(LUCES_GPIO_PORT, ROJOA_Pin, GPIO_PIN_RESET);
		break;

	case AMARILLO_A:
		HAL_GPIO_WritePin(LUCES_GPIO_PORT, AMARILLOA_Pin, GPIO_PIN_RESET);
		break;

	case VERDE_A:
		HAL_GPIO_WritePin(LUCES_GPIO_PORT, VERDEA_Pin, GPIO_PIN_RESET);
		break;

	default:
		Funcionapagartodos();
	}

}

void apagarB(SecuenciaB estadoActualB) {
	switch(estadoActualB){
	case ROJO_B:
		HAL_GPIO_WritePin(LUCES_GPIO_PORT, ROJOB_Pin, GPIO_PIN_RESET);
		break;

	case AMARILLO_B:
		HAL_GPIO_WritePin(LUCES_GPIO_PORT, AMARILLOB_Pin, GPIO_PIN_RESET);
		break;

	case VERDE_B:
		HAL_GPIO_WritePin(LUCES_GPIO_PORT, VERDEB_Pin, GPIO_PIN_RESET);
		break;

	default:
		Funcionapagartodos();
	}

}

void encenderA(SecuenciaA estadoActualA) {
	switch(estadoActualA){
	case ROJO_A:
		HAL_GPIO_WritePin(LUCES_GPIO_PORT, ROJOA_Pin, GPIO_PIN_SET);
		break;

	case AMARILLO_A:
		HAL_GPIO_WritePin(LUCES_GPIO_PORT, AMARILLOA_Pin, GPIO_PIN_SET);
		break;

	case VERDE_A:
		HAL_GPIO_WritePin(LUCES_GPIO_PORT, VERDEA_Pin, GPIO_PIN_SET);
		break;

	default:
		Funcionencendertodos();
	}

}

void encenderB(SecuenciaB estadoActualB) {
	switch(estadoActualB){
	case ROJO_B:
		HAL_GPIO_WritePin(LUCES_GPIO_PORT, ROJOB_Pin, GPIO_PIN_SET);
		break;

	case AMARILLO_B:
		HAL_GPIO_WritePin(LUCES_GPIO_PORT, AMARILLOB_Pin, GPIO_PIN_SET);
		break;

	case VERDE_B:
		HAL_GPIO_WritePin(LUCES_GPIO_PORT, VERDEB_Pin, GPIO_PIN_SET);
		break;

	default:
		Funcionencendertodos();
	}

}

SecuenciaA avanzarA(SecuenciaA estadoActualA) {
	apagarA(estadoActualA);
	SecuenciaA estadoSiguienteA = (SecuenciaA)((estadoActualA + 1) % TOTAL_SECUENCIAA);
	encenderA(estadoSiguienteA);
	return estadoSiguienteA;
}

SecuenciaB avanzarB(SecuenciaB estadoActualB) {
	apagarB(estadoActualB);
	SecuenciaB estadoSiguienteB = (SecuenciaB)((estadoActualB + 1) % TOTAL_SECUENCIAB);
	encenderB(estadoSiguienteB);
	return estadoSiguienteB;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	uint32_t now = HAL_GetTick();

	if (GPIO_Pin == SW1_Pin) {
		if ((now - lastDebounceTickSw1) >= TREBOTE) {
			solicitud = 1;
			lastDebounceTickSw1 = now;
		}
	}
	else if (GPIO_Pin == SW2_Pin) {
		if ((now - lastDebounceTickSw2) >= TREBOTE) {
			if (HAL_GPIO_ReadPin(SW2_GPIO_Port, SW2_Pin) == GPIO_PIN_SET) {
				solicitud = 2;
				lastDebounceTickSw2 = now;
			}
		}
	}
	else if (GPIO_Pin == SW3_Pin) {
		if ((now - lastDebounceTickSw3) >= TREBOTE) {
			solicitud = 3;
			lastDebounceTickSw3 = now;
		}
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
