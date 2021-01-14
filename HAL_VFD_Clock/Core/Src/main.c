
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

#include "display.h"

void SystemClock_Config(void);

/* @brief TIM3 period elapsed callback - multiplexing
 *
 * Callback of timer 3 that is triggered in 1ms interval. It calls function that operates display multiplexer.
 * Every 1ms different next is powered and next character is displayed.
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	multiplexerSequence();
}

int main(void)
{
	HAL_Init();
	SystemClock_Config();

	MX_GPIO_Init();
	MX_DMA_Init();
	MX_I2C2_Init();
	MX_TIM1_Init();
	MX_USART1_UART_Init();
	MX_TIM2_Init();
	MX_TIM3_Init();

	// Start timer for buzzer
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 0);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

	// Start timer for VFD tube H-bridge powering
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 500);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);

	// Start timer for multiplexing
	HAL_TIM_Base_Start_IT(&htim3);

	HAL_Delay(1000);

	char numbers[] = "1234567890";
	char hello[] = " hell0  ";
	char date[] = " 2012020";

	while (1)
	{
		dispString(numbers, 7);
		HAL_Delay(1000);
		dispString(hello, 7);
		HAL_Delay(1000);
		dispString(date, 7);
		tab_to_display[4] |= mask_dot;
		tab_to_display[6] |= mask_dot;
		HAL_Delay(1000);
		dispString("    23*c  ", 7);
		HAL_Delay(5000);
		dispString("h 45 pro", 7);
		HAL_Delay(5000);
	}
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
	if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	{
		Error_Handler();
	}
}


/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
