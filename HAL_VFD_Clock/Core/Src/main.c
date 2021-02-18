#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

#include "display.h"
#include "DS3231.h"

#define MENU_WATCHDOG_MAX 3000

void SystemClock_Config(void);                                  //Clock source and PLL init
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);    //Multiplexer timer callback
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
bool isLeapYear(int32_t year);
uint8_t daysInMonth(_RTC *local_RTC);
void change_time(_RTC *local_RTC);


volatile uint8_t SEL_flag = 0;
volatile uint8_t PLUS_flag = 0;
volatile uint8_t MINUS_flag = 0;

volatile int32_t menu_watchdog = 0;
volatile uint8_t RTC_saved_flag = 1;

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
	//reset blink mask
	blink_mask = 0b11111111;

	// Init DS3231 RTC
	_RTC local_RTC;
	DS3231_Init(&hi2c2);

	dispString("hell0", 5);
	HAL_Delay(2000);

	//Display temperature form RTC. Useful for checking if circuit doesn't overheat insice casing.
	float RTC_temperature;
	DS3231_ReadTemperature(&RTC_temperature);
	displayTemperature(RTC_temperature);
	HAL_Delay(2000);

	while (1)
	{
		//normal operation mode - display time and date
		if(RTC_saved_flag == 1)
		{
			static uint16_t date_or_hour_timer = 0;

			DS3231_GetTime(&local_RTC);
			if(date_or_hour_timer < 200)
			{
				displayDate(local_RTC.Date, local_RTC.Month, local_RTC.Year);
			}
			else
			{
				displayHour(local_RTC.Hour, local_RTC.Min, local_RTC.Sec);
			}
			date_or_hour_timer++;
			if(date_or_hour_timer == 1200)
				date_or_hour_timer = 0;
			HAL_Delay(10);
		}
		//menu mode - date and time setup
		if(SEL_flag == 1)
		{
			RTC_saved_flag = 0;
			change_time(&local_RTC);
		}
	}
}

/* @brief TIM3 period elapsed callback - multiplexing
 *
 * Callback of timer 3 that is triggered in 1ms interval. It calls function that operates display multiplexer.
 * Every 1ms different next is powered and next character is displayed.
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	multiplexerSequence();
}

/* @brief External GPIO Interrupt Callback
 *
 * Callback of buttons and RTC EXTI events. Sets event flags to true;
 *
 * @param[in] GPIO_Pin number of GPIO pin that caused interrupt
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);

	if(GPIO_Pin == BUTTON_SELECT_Pin)
		SEL_flag = 1;
	if(GPIO_Pin == BUTTON_PLUS_Pin)
		PLUS_flag = 1;
	if(GPIO_Pin == BUTTON_MINUS_Pin)
		MINUS_flag = 1;
}
/*
 * @brief Checks if given year has 29 days in February
 *
 * @param[in] year year in full decimal format
 */
bool isLeapYear(int32_t year)
{
	if((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
		return true;
	else
		return false;
}

/*
 * @brief Returns number of days in current month.
 *
 * Checks for leap years and returns 29 for February in such year.
 *
 * @param[in] local_RTC pointer to structure with RTC date and time
 */

uint8_t daysInMonth(_RTC *local_RTC)
{
	switch (local_RTC->Month)
	{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			return 31;
		case 6:
		case 9:
		case 11:
			return 30;
		case 2:
		{
			if(isLeapYear(local_RTC->Year))
				return 29;
			else
				return 28;
		}
	}
	return 0;
}

void change_time(_RTC *local_RTC)
{
	//reset buttons flag - just to be sure they are 0
	SEL_flag = 0;
	PLUS_flag = 0;
	MINUS_flag = 0;

	/* variable keeps ID of currently selected menu entry:
	0 - hours
	1 - minutes
	2 - seconds
	3 - day
	4 - month
	5 - year
	6 - exit from menu
	*/
	uint8_t current_selection = 0;

	/* Watchdog variable is decremented every loop cycle. It is restored to original value
	 * if any button was pressed before it reaches zero.
	 */
	menu_watchdog = MENU_WATCHDOG_MAX;

	while (menu_watchdog > 0)
	{
		//decide what to display - clock (time) or calendar (date)
		if(current_selection < 3)
			displayHour(local_RTC->Hour, local_RTC->Min, local_RTC->Sec);
		else if(current_selection >= 3)
			displayDate(local_RTC->Date, local_RTC->Month, local_RTC->Year);

		if(SEL_flag == 1)
		{
			SEL_flag = 0;                      //reset button flag
			current_selection++;               //increment menu entry ID
			menu_watchdog = MENU_WATCHDOG_MAX; //reset watchdog
			if(current_selection == 6)         //if menu was skipped, set watchdog to 0 and brake loop in next iteration
				menu_watchdog = 0;

		}

		//change hours
		if(PLUS_flag == 1 && current_selection == 0)
		{
			//increment hours ------------------------------------------------------------------------------
			if(local_RTC->Hour >= 23)
				local_RTC->Hour = 0;
			else
				local_RTC->Hour++;
			menu_watchdog = MENU_WATCHDOG_MAX;
			PLUS_flag = 0;
		}
		if(MINUS_flag == 1 && current_selection == 0)
		{
			//decrement hours
			if(local_RTC->Hour == 0)
				local_RTC->Hour = 23;
			else
				local_RTC->Hour--;
			menu_watchdog = MENU_WATCHDOG_MAX;
			MINUS_flag = 0;
		}

		//change minutes ------------------------------------------------------------------------------
		if(PLUS_flag == 1 && current_selection == 1)
		{
			//increment minutes
			if(local_RTC->Min >= 59)
				local_RTC->Min = 0;
			else
				local_RTC->Min++;
			menu_watchdog = MENU_WATCHDOG_MAX;
			PLUS_flag = 0;
		}
		if(MINUS_flag == 1 && current_selection == 1)
		{
			//decrement minutes
			if(local_RTC->Min == 0)
				local_RTC->Min = 59;
			else
				local_RTC->Min--;
			menu_watchdog = MENU_WATCHDOG_MAX;
			MINUS_flag = 0;
		}

		//change seconds ------------------------------------------------------------------------------
		if(PLUS_flag == 1 && current_selection == 2)
		{
			//increment seconds
			if(local_RTC->Sec >= 59)
				local_RTC->Sec = 0;
			else
				local_RTC->Sec++;
			menu_watchdog = MENU_WATCHDOG_MAX;
			PLUS_flag = 0;
		}
		if(MINUS_flag == 1 && current_selection == 2)
		{
			//decrement seconds
			if(local_RTC->Sec == 0)
				local_RTC->Sec = 59;
			else
				local_RTC->Sec--;
			menu_watchdog = MENU_WATCHDOG_MAX;
			MINUS_flag = 0;
		}

		//change day of month ------------------------------------------------------------------------------
		if(PLUS_flag == 1 && current_selection == 3)
		{
			//increment day if current month has 31 days
			if(local_RTC->Date >= daysInMonth(local_RTC))
				local_RTC->Date = 1;
			else
				local_RTC->Date++;
			menu_watchdog = MENU_WATCHDOG_MAX;
			PLUS_flag = 0;
		}
		if(MINUS_flag == 1 && current_selection == 3)
		{
			//decrement day
			if(local_RTC->Date == 1)
				local_RTC->Date = daysInMonth(local_RTC);
			else
				local_RTC->Date--;
			menu_watchdog = MENU_WATCHDOG_MAX;
			MINUS_flag = 0;
		}

		//change month ------------------------------------------------------------------------------
		if(PLUS_flag == 1 && current_selection == 4)
		{
			//increment month
			if(local_RTC->Month >= 12)
				local_RTC->Month = 1;
			else
				local_RTC->Month++;
			menu_watchdog = MENU_WATCHDOG_MAX;
			PLUS_flag = 0;
		}
		if(MINUS_flag == 1 && current_selection == 4)
		{
			//decrement month
			if(local_RTC->Month == 1)
				local_RTC->Month = 12;
			else
				local_RTC->Month--;
			menu_watchdog = MENU_WATCHDOG_MAX;
			MINUS_flag = 0;
		}

		//change year ------------------------------------------------------------------------------
		if(PLUS_flag == 1 && current_selection == 5)
		{
			//increment year
			if(local_RTC->Year >= 99)
				local_RTC->Year = 99;
			else
				local_RTC->Year++;
			menu_watchdog = MENU_WATCHDOG_MAX;
			PLUS_flag = 0;
		}
		if(MINUS_flag == 1 && current_selection == 5)
		{
			//decrement year
			if(local_RTC->Year == 0)
				local_RTC->Year = 0;
			else
				local_RTC->Year--;

			menu_watchdog = MENU_WATCHDOG_MAX;
			MINUS_flag = 0;
		}

		//blink active selection
		if(current_selection == 0)
			blink_mask = 0b00111111;  //blink first two digits for hour selection
		if(current_selection == 1)
			blink_mask = 0b11100111;  //blink middle two digits for minutes selection
		if(current_selection == 2)
			blink_mask = 0b11111100;  //blink last two digits for second selection
		if(current_selection == 3)
			blink_mask = 0b00111111;  //blink first two digits for day of month selection
		if(current_selection == 4)
			blink_mask = 0b11001111;  //blink second pair of digits for month selection
		if(current_selection == 5)
			blink_mask = 0b11110000;  //blink last 4 digits for year selection

		HAL_Delay(1);
		menu_watchdog--;
	}
	DS3231_SetTime(local_RTC);
	RTC_saved_flag = 1;
	blink_mask = 0b11111111;
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
