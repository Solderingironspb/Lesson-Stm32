/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdbool.h>
#define size_tx_buffer 10 //����� ������ tx
bool flag_send; //���� �� ���������� ������ tx_buffer[] � serial port
uint8_t queue_message; //������� ��������� (�.�. �������� � ������������ ��� DMA. ��. ���� 6)
uint8_t tx_buffer[size_tx_buffer]; //���������� tx_buffer, ������� ����� �������� � ���� ��, ��� ������ �� �� rx.
uint8_t rx_buffer[1]; // �����, ������� ����� ��������� ����������. (�� ������ �������, ���� ��������� � ���������� �������� � ������������ �������� ���������.)
uint8_t error_message[] = "tx buffer is crowded\n\r"; //��������� � ���, ��� tx_buffer[size_tx_buffer] ����������.
uint8_t error_counter; // ������ ��� ����, ���� ��������� "tx buffer is crowded\n\r" ���������� ������ ���.
volatile uint8_t rx_counter; // ������� ��� �������� ��������.
volatile uint8_t size_message; // ������ ��������� ��� ������ � ����.
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
UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart1_rx;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {//������ ��������, ����������� ����������, ����� �� ����� ��� ���-�� ������ � ��������� �������.
	if (rx_buffer[0] != '\r') { //���� �������� ������ �� ����� '\r'
		tx_buffer[rx_counter] = rx_buffer[0]; //������� ���� ������ � tx_buffer
		rx_counter++;//����������� ������� �������� ��������
		size_message = rx_counter; //������ ���������, ������� � ������ ���� ����� ������������
		if (size_message >= size_tx_buffer && error_counter == 0) {//���� ������ ��������� ��������� ������ ������, �� ������� ���������, ��� ����� ����������.
			HAL_UART_Transmit_DMA(&huart1, error_message,
					sizeof error_message / sizeof error_message[0]);
			error_counter++;//������� ������ ��������� ������. ���� ����������� ������ ���.
			flag_send = 0;//��������� �������� ��������� � ����, ���� ��� ��������� �� �������.
		}
	} else if (rx_buffer[0] == '\r') {//���� �� �������� ������ ����� '\r'
		tx_buffer[rx_counter] = '\n';//�� ���� ��������� ��������� ��������. ������� ������� � ���� ������� �� ��������� ������
		tx_buffer[rx_counter + 1] = '\r';//��� �� ������� ������� �������
		tx_buffer[rx_counter + 2] = '\0';//������� �������� ������� ������, �.�. �� ��� ���� �� �������
		size_message = rx_counter + 3;//�������������� �� ������ ��������� ������ ������������� ��������� � ����.
		rx_counter = 0;//������� ������� �������� ��������, ���� ������ �������� ���������.
		if (size_message >= size_tx_buffer) {//����� ��, ���� ������ ��������� ��������� ������ ������, �� ������� ���������, ��� ����� ����������.
			flag_send = 0;////��������� �������� ��������� � ����, ���� ��� ��������� �� �������.
			HAL_UART_Transmit_DMA(&huart1, error_message,
					sizeof error_message / sizeof error_message[0]);
		} else {
			flag_send = 1;//���� �� ��������� �� ��������� �� ������� tx_buffer, �� ��������� �������� � ���� ��, ��� ������
			error_counter = 0;//������� ������� ������
		}
	}
	HAL_UART_Receive_IT(&huart1, rx_buffer, 1);//��������� ����� ������ ����� ������� ����������.
}
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

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_USART1_UART_Init();
	/* USER CODE BEGIN 2 */
	flag_send = 0;//�������� ��������� ��� ���������, ���� ���������� ��� ������.
	queue_message = 1;//������� �������� ��������� ���������� ����� 1
	rx_counter = 0;//������� ��� �� ���������. ����� 0.
	size_message = 0;//������� ������������� ��������� ��� �� �����. ����� 0.
	error_counter = 0;//������ ��� �� ����. ����� 0.
	HAL_UART_Receive_IT(&huart1, rx_buffer, 1);//��������� ����� ������, �������� � 1 ������.
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */

		if (flag_send == 1 && queue_message != 255) {//���� ��������� �������� ��������� � ������� �� ����� ������������, ��
				if (queue_message == 1 && huart1.gState == HAL_UART_STATE_READY) { //���� ���� ��������, � ������� ��������� �������������, ��
				HAL_UART_Transmit_DMA(&huart1, tx_buffer, size_message);//���������� ���������, ���� ��� ��� ������ � rx_buffer
				queue_message = 255;//������ ��������� �������, ���� ������� ��� ���-�� �������� � ����.
			}
		} else if (flag_send == 1 && queue_message == 255) {//���� �� ������� ����� �� ������������, ��
			queue_message = 1;//������� ������� ��������, ���� ��������� ����� ����� �� ������� ����������
			flag_send = 0;//�������� ��������� ��������. ���������� ������� � ����������.
		}
		if (tx_buffer[0] == 'O' && tx_buffer[1] == 'n' && tx_buffer[2] == '\n' && tx_buffer[3] == '\r') { //���� � tx_buffer ������ 'On\r', ��
			GPIOC->BRR = LED_Pin; //������� 13 ���������
		} else if (tx_buffer[0] == 'O' && tx_buffer[1] == 'f' && tx_buffer[2] == 'f' && tx_buffer[3] == '\n' && tx_buffer[4] == '\r') { //���� � tx_buffer ������ 'Off\r', ��
			GPIOC->BSRR = LED_Pin; //�������� 13 ���������
		}
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

	/** Initializes the CPU, AHB and APB busses clocks
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB busses clocks
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
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART1_UART_Init(void) {

	/* USER CODE BEGIN USART1_Init 0 */

	/* USER CODE END USART1_Init 0 */

	/* USER CODE BEGIN USART1_Init 1 */

	/* USER CODE END USART1_Init 1 */
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART1_Init 2 */

	/* USER CODE END USART1_Init 2 */

}

/** 
 * Enable DMA controller clock
 */
static void MX_DMA_Init(void) {

	/* DMA controller clock enable */
	__HAL_RCC_DMA1_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA1_Channel4_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
	/* DMA1_Channel5_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);

	/*Configure GPIO pin : LED_Pin */
	GPIO_InitStruct.Pin = LED_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
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
