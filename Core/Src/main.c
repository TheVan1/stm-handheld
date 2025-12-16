/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
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
#include "string.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
#define DISPLAY_X_SIZE 128
#define DISPLAY_Y_SIZE 64
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */
void I2C_SSD1306_Screen_Init(void);
void I2C_SSD1306_Update_Whole_Display(uint8_t SSD1306_FrameBufferPages[128][8]);
static void I2C_SSD1306_Screen_Transmit(uint16_t data_len, uint8_t *data);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */
	//a buffer of the current view on the screen
	//each bit corresponds to a VERTICAL section of 8 (single bit) pixels
	uint8_t SSD1306_FrameBufferPages[128][8] = { { } };

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_USART2_UART_Init();
	MX_I2C1_Init();
	/* USER CODE BEGIN 2 */
	I2C_SSD1306_Screen_Init();

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */

		//testing values, swap between a full white and full black screen every second
		memset(SSD1306_FrameBufferPages, 0b00000000, 1024);
		HAL_Delay(1000);
		I2C_SSD1306_Update_Whole_Display(SSD1306_FrameBufferPages);

		memset(SSD1306_FrameBufferPages, 0b11111111, 1024);
		HAL_Delay(1000);
		I2C_SSD1306_Update_Whole_Display(SSD1306_FrameBufferPages);
		continue;

	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 16;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void) {

	/* USER CODE BEGIN I2C1_Init 0 */

	/* USER CODE END I2C1_Init 0 */

	/* USER CODE BEGIN I2C1_Init 1 */

	/* USER CODE END I2C1_Init 1 */
	hi2c1.Instance = I2C1;
	hi2c1.Init.ClockSpeed = 400000;
	hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c1.Init.OwnAddress1 = 128;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN I2C1_Init 2 */
	/* USER CODE END I2C1_Init 2 */

}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void) {

	/* USER CODE BEGIN USART2_Init 0 */

	/* USER CODE END USART2_Init 0 */

	/* USER CODE BEGIN USART2_Init 1 */

	/* USER CODE END USART2_Init 1 */
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart2) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART2_Init 2 */

	/* USER CODE END USART2_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	/* USER CODE BEGIN MX_GPIO_Init_1 */

	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin : B1_Pin */
	GPIO_InitStruct.Pin = B1_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : LD2_Pin */
	GPIO_InitStruct.Pin = LD2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

	/* USER CODE BEGIN MX_GPIO_Init_2 */

	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

//TODO: HIGH PRIORITY; spin all SSD1306 functions into their own file, and integrate via header files
//INFO: all SSD1306 functions rely on a 128x8, uint8 array. in this array, each individual *bit* is a single pixel
//TODO: ISR call of Update_Whole_Display X times per second
//TODO: write functions to draw squares, lines, circles etc.
//TODO: LOW PRIORITY; less naive integration of drawing functions to speed up drawing
//(currently speed is adequate, must do testing on speediness before rewrite drawing.
/**
 * @brief I2C initilisation of a small OLED screen
 * @param None
 * @retval None
 */
void I2C_SSD1306_Screen_Init(void) {
	//Reference manual for initialisation sequence can be found here (https://www.alldatasheet.com/datasheet-pdf/view/1179026/ETC2/SSD1306.html)

	uint8_t data[2];

	// Set the MUX ratio (0xA8) to 0x3F
	data[0] = 0x80;
	data[1] = 0xA8;
	I2C_SSD1306_Screen_Transmit((uint16_t) 2, data);
	data[0] = 0x80;
	data[1] = 0x3F;
	I2C_SSD1306_Screen_Transmit((uint16_t) 2, data);

	// Set the Display Offset (0xD3) to 0x00
	data[0] = 0x80;
	data[1] = 0xD3;
	I2C_SSD1306_Screen_Transmit((uint16_t) 2, data);
	data[0] = 0x80;
	data[1] = 0x00;
	I2C_SSD1306_Screen_Transmit((uint16_t) 2, data);

	// Set Display Start Line (0x40)
	data[0] = 0x80;
	data[1] = 0x40;
	I2C_SSD1306_Screen_Transmit((uint16_t) 2, data);

	// Set Segment re-map (0xA1)
	data[0] = 0x80;
	data[1] = 0xA1;
	I2C_SSD1306_Screen_Transmit((uint16_t) 2, data);

	// I2C_SSD1306_Screen_Transmit(ection (0xC8)
	data[0] = 0x80;
	data[1] = 0xC8;
	I2C_SSD1306_Screen_Transmit((uint16_t) 2, data);

	// Set COM hardware configuration (0xDA) to 0x12
	data[0] = 0x80;
	data[1] = 0xDA;
	I2C_SSD1306_Screen_Transmit((uint16_t) 2, data);
	data[0] = 0x80;
	data[1] = 0x12;
	I2C_SSD1306_Screen_Transmit((uint16_t) 2, data);

	// Set Contrast Control (0x81) to 0x7F
	data[0] = 0x80;
	data[1] = 0x81;
	I2C_SSD1306_Screen_Transmit((uint16_t) 2, data);
	data[0] = 0x80;
	data[1] = 0x7F;
	I2C_SSD1306_Screen_Transmit((uint16_t) 2, data);

	// Set Disable Entire Display On (0xA4)
	data[0] = 0x80;
	data[1] = 0xA4;
	I2C_SSD1306_Screen_Transmit((uint16_t) 2, data);

	// Set Normal Display (0xA6)
	data[0] = 0x80;
	data[1] = 0xA6;
	I2C_SSD1306_Screen_Transmit((uint16_t) 2, data);

	// Set OSC Frequency (0xD5) to 0x80
	data[0] = 0x80;
	data[1] = 0xD5;
	I2C_SSD1306_Screen_Transmit((uint16_t) 2, data);
	data[0] = 0x80;
	data[1] = 0x80;
	I2C_SSD1306_Screen_Transmit((uint16_t) 2, data);

	// Set Enable Charge Pump Regulator (0xAD) to 0x14
	data[0] = 0x80;
	data[1] = 0x8D;
	I2C_SSD1306_Screen_Transmit((uint16_t) 2, data);
	data[0] = 0x80;
	data[1] = 0x14;
	I2C_SSD1306_Screen_Transmit((uint16_t) 2, data);

	// Set Display On (0xAF)
	data[0] = 0x80;
	data[1] = 0xAF;
	I2C_SSD1306_Screen_Transmit((uint16_t) 2, data);

	// Set Direction for pages (0x20) to columns (0x01)
	data[0] = 0x80;
	data[1] = 0x20;
	I2C_SSD1306_Screen_Transmit((uint16_t) 2, data);
	data[0] = 0x80;
	data[1] = 0x01;
	I2C_SSD1306_Screen_Transmit((uint16_t) 2, data);

}

/**
 * @brief transmission of data over I2C to OLED screen
 * @param data_len number of frames to transmit, data actual data to transmit
 * @retval None
 */

void I2C_SSD1306_Update_Whole_Display(uint8_t SSD1306_FrameBufferPages[128][8]) {
	// Set the columns and pages to be written to

	// 0x80 - Control byte
	// 0x21 - Set Column
	// 0x80 - Control byte
	// 0x0 - Set starting column to 0
	// 0x80 - Control byte
	// 127 - Set end column to 127
	// ---------------------------
	// 0x80 - Control byte
	// 0x22 - Set page
	// 0x80 - Control byte
	// 0x0 - Set starting page to 0
	// 0x80 - Control byte
	// 0x7 - Set end page to 7

	uint8_t command_data[12] = { 0x80, 0x21, 0x80, 0x0, 0x80, 127, 0x80, 0x22,
			0x80, 0x00, 0x80, 0x07 };

	// Create an array to hold the data to send to the display + 1 (byte to indicate data transmission, which is 0x40)
	uint8_t data[1025] = { 0 };
	data[0] = 0x40;

	// For each of the columns 0 to 127
	for (volatile uint8_t x = 0; x < 128; x++) {
		// For each of the pages, 0 to 7
		for (volatile uint8_t y = 0; y < 8; y++) {
			//retrieve the current page from the 2d array, and write that into the 1d array for transmission
			uint8_t current_page = SSD1306_FrameBufferPages[x][y];

			//8*128 + 1 = 1025
			//we skip the first byte as that is 0x40, to declare data transmission
			data[((y * 128) + x) + 1] = (uint8_t) current_page;
		}
	}

	I2C_SSD1306_Screen_Transmit((uint16_t) 12, command_data);
	I2C_SSD1306_Screen_Transmit((uint16_t) 1025, data);

	return;
}

static void I2C_SSD1306_Screen_Transmit(uint16_t data_len, uint8_t *data) {
	HAL_I2C_Master_Transmit(&hi2c1, ((uint16_t) 0x3c) << 1, data, data_len,
			100);
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
	/* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
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
