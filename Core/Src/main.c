/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

//#define  TEST_PC_USB
//#define TEST_BUTTON
//#define TEST_FLASH
#define TEST_BOOTLOADER
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

//#include "usbd_cdc_if.h"
#include "hal_PC.h"
#include "appcb_PC.h"
#include "hal_Button.h"
#include "appcb_Button.h"
#include "hal_Flash.h"
#include "app_PCHelper.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
#ifdef TEST_PC_USB
char *g_pui8Data= " Hello from Rahma's PC";
uint8_t g_aui8PCBuffer[64];
#endif

#ifdef TEST_FLASH
//uint8_t g_aui8FlashTXBuffer[8]={1,2,3,4,5,6,7,8};
//uint8_t g_aui8FlashRXBuffer[1024];

uint8_t data2[] = {0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9};

uint8_t Rx_Data[30];

//uint8_t data2[] = {0x1,0x2,0x3,0x4,0x5,0x6,0x7,0x8,0x9};
//
//uint8_t Rx_Data[30];
#endif

//#ifdef TEST_BOOTLOADER
//uint32_t g_ui3StartAddData=ADDR_FLASH_PAGE_13;
//#endif

//#ifdef  TEST_BUTTON
//uint8_t flag=0;
//#endif


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
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
#ifdef  TEST_BUTTON
	HAL_Button_Init(GPIO_PIN_13,APPCB_Button);
#endif

#ifdef  TEST_PC_USB
	HAL_PC_init(APPCB_PC_MsgReceived,APPCB_PC_MsgSent);
#endif

#ifdef  TEST_BOOTLOADER

	HAL_Button_Init(GPIO_PIN_13,APPCB_Button);

	HAL_PC_init(APPCB_PC_MsgReceived,APPCB_PC_MsgSent);

//	HAL_Flash_Write_Data(ADDR_FLASH_PAGE_255, &g_ui3StartAddData, PCHELPER_MAX_MSG_SIZE_IN_BYTES);

#endif

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */
  //	CDC_Transmit_FS((uint8_t*) g_pui8Data,strlen (g_pui8Data));
  	//	HAL_Delay(1000);


  #ifdef TEST_FLASH
  	//	HAL_FLASH_Write((uint8_t *)data2, 9 );
  	//	HAL_FLASH_Read(Rx_Data,10);
  	HAL_Flash_Write_Data(ADDR_FLASH_PAGE_13 , (uint32_t *)data2, 9);

  	memset(Rx_Data,0,30);

  	HAL_Flash_Read_Data(ADDR_FLASH_PAGE_13 ,(uint32_t *) Rx_Data, 10);
  #endif
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
#ifdef TEST_BOOTLOADER

		APP_USBBOOTLOADER_Init();
#endif
#ifdef TEST_PC_USB
		HAL_PC_write((uint8_t*) g_pui8Data,strlen (g_pui8Data));
		HAL_Delay(3000);

		//loop till PC Rx Channel Idle is Yes
		while(g_ui8PCRxChannelIdle == NO);

		// Set PC Rx Channel Idle to No
		g_ui8PCRxChannelIdle = NO;
		HAL_PC_read(g_aui8PCBuffer,g_ui8PCRxMsgLen);
		HAL_Delay(1000);
#endif

#ifdef  TEST_BUTTON
		if (g_etui8BtnState==PRESSED)
		{
			HAL_Delay(30);
			if (g_etui8BtnState==PRESSED )
			{

				HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);
				//					flag=1;
				//					HAL_Delay(5000);
			}

		}
		else if (g_etui8BtnState==RELEASED )
		{
			HAL_Delay(30);
			if (g_etui8BtnState==RELEASED )
			{
				HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
			}

		}






#endif

		//		CDC_Transmit_FS((uint8_t*) g_pui8Data,strlen (g_pui8Data));
		//		HAL_Delay(1000);

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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 60;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin : PF13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

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

