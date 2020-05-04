/* USER CODE BEGIN Header */

/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "audio_format.h"
#include "debug_helper.h"
#include "fft.h"
#include "corr.h"
#include "median.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DEBUG_MODE
#define    DWT_CYCCNT    *(volatile uint32_t*)0xE0001004
#define    DWT_CONTROL   *(volatile uint32_t*)0xE0001000
#define    SCB_DEMCR     *(volatile uint32_t*)0xE000EDFC
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
Audio_Data adc_data = {
		.len = AUDIO_DATA_LEN
};

Audio_Data adc_data_2 = {
		.len = AUDIO_DATA_LEN
};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

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
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_USART1_UART_Init();
  MX_TIM8_Init();
  /* USER CODE BEGIN 2 */
//  HAL_OPAMP_Start(&hopamp1);
  Audio_Data *current_buffer = &adc_data;
  char string[128] = {};
  size_t bytes_written = sprintf(string, "\n\rADC Frequency: %d\n\r", ADC_FREQUENCY);
  HAL_UART_Transmit(&huart1, string, bytes_written, 100);
  int there_was_whistle = 0;
  // calibration
  HAL_ADC_Start(&hadc2);
  HAL_ADCEx_MultiModeStart_DMA(&hadc1, (uint32_t*)current_buffer, adc_data.len);
  HAL_Delay(40);
  SCB_DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;// разрешаем использовать DWT



  calibrate_mean(current_buffer);
  bytes_written = sprintf(string, "Silence pressure: %d, %d\n\r", mean_silence_ch1, mean_silence_ch2);
  HAL_UART_Transmit(&huart1, string, bytes_written, 100);
  HAL_TIM_Base_Start(&htim8);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  while(1) {
		  HAL_Delay(300);
	  }
	  HAL_ADC_Stop(&hadc2);
	  if(current_buffer == &adc_data) {
		  current_buffer = &adc_data_2;
	  } else {
		  current_buffer = &adc_data;
	  }
	  led_off();
	  HAL_ADC_Start(&hadc2);
	  HAL_ADCEx_MultiModeStop_DMA(&hadc1);
	  HAL_ADCEx_MultiModeStart_DMA(&hadc1, (uint32_t*)current_buffer, adc_data.len);
	  // time laps
	   DWT_CONTROL|= DWT_CTRL_CYCCNTENA_Msk; // включаем счётчик
	   DWT_CYCCNT = 0;// обнуляем счётчик
	   q15_t* corr_arr;
	   if(!is_silence(current_buffer == &adc_data ? &adc_data_2 : &adc_data)) {
		   corr_arr = eval_shift(current_buffer == &adc_data ? &adc_data_2 : &adc_data);
		   int shift_dx = max_index(corr_arr);
		   median_add(shift_dx);
			  int timerValue = DWT_CYCCNT;
//		  	  print_debug_int(timerValue);

//		   if(shift_dx <= 0 ){
//			   	   print_debug_int(timerValue);
//			   	   HAL_Delay(160);
//				  print_debug_ch1(current_buffer == &adc_data ? &adc_data_2 : &adc_data);
//				  print_debug_ch2(current_buffer == &adc_data ? &adc_data_2 : &adc_data);
//				  print_debug_array(corr_arr, 512);
//		   }

	   }

	   int filtered_idx;
	   if(median_current_size() == MEDIAN_LEN) {
  		  filtered_idx = median_value();
		  led_peleng(to_grad(filtered_idx));

  		  print_debug_int(filtered_idx);
//  		  print_debug_array(corr_arr, 512);
//  		  print_debug_ch1(current_buffer == &adc_data ? &adc_data_2 : &adc_data);
//  		  print_debug_ch2(current_buffer == &adc_data ? &adc_data_2 : &adc_data);
  	  }



  	 while(HAL_DMA_GetState(&hdma_adc1) == HAL_DMA_STATE_BUSY){
  			  HAL_Delay(1);
  	 }





    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV8;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV16;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV16;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_TIM8;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_SYSCLK;
  PeriphClkInit.Tim8ClockSelection = RCC_TIM8CLK_HCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
