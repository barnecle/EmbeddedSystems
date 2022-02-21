/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include <stdlib.h>
#include <stdio.h>
#include "stm32l4xx_ll_usart.h"
#include <string.h>
#include <retarget.h>
#include <command.h>
#include "queue.h"
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
RTC_HandleTypeDef hrtc;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
queue_t rx_queue;
int led_on = 0;
volatile int toggle = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_RTC_Init(void);
/* USER CODE BEGIN PFP */
int help_command(char *args);
int lof_command();
int lon_command();
int test_command(char *args);
int dateSet_command(char *args);
int timeSet_command(char *args);
void printDateTime();
int parse_command (uint8_t *line, uint8_t **command, uint8_t **args);
int execute_command(uint8_t * line);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
command_t commands[] = {
  {"help",help_command},
  {"lof",lof_command},
  {"lon",lon_command},
  {"test",test_command},
  {"ds",dateSet_command},
  {"ts",timeSet_command},
  {0,0}
};


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
  uint8_t command_line[QUEUE_SIZE];
  uint8_t ch;
  //RTC_DateTypeDef date;
  //RTC_TimeTypeDef time;
  int n = 0;
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */
  RetargetInit(&huart2);
  LL_USART_EnableIT_RXNE(USART2);
  printf("\r\nSystem Running\n\r");
  printf("\r\nSTM$");
  //date.WeekDay = 1;
  //date.Month = 1;
  //date.Date = 1;
 // date.Year = 1;
  //time.Hours = 1;
  //time.Minutes = 1;
  //HAL_RTC_SetDate(&hrtc,&date,RTC_FORMAT_BIN);
  //HAL_RTC_SetTime(&hrtc,&time,RTC_FORMAT_BIN);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	if(toggle){
		HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
		HAL_Delay(100);
		HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
		toggle = 0;
	}
    if ((ch = dequeue(&rx_queue))) {
      putchar(ch);
      command_line[n] = ch;
      if(ch == '\r' || ch == '\n'){
	command_line[n] = '\0'; //end of string
      	if(!execute_command(command_line)){
	  printf("\r\nOK");
	}else{
	  printf("\r\nNOK");
	}
	n = 0;
	printf("\r\n");
	printDateTime();
	printf(" IULS>");
      }else{
	n++;
      }

    }
  }


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_LSE
                              |RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_10;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
  RTC_AlarmTypeDef sAlarm = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  hrtc.Init.OutPutPullUp = RTC_OUTPUT_PULLUP_NONE;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x1;
  sDate.Year = 0x0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enable the Alarm A
  */
  sAlarm.AlarmTime.Hours = 0x0;
  sAlarm.AlarmTime.Minutes = 0x0;
  sAlarm.AlarmTime.Seconds = 0x30;
  sAlarm.AlarmTime.SubSeconds = 0x0;
  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  sAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY|RTC_ALARMMASK_HOURS
                              |RTC_ALARMMASK_MINUTES;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm.AlarmDateWeekDay = 0x1;
  sAlarm.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enable the WakeUp
  */
  if (HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 0, RTC_WAKEUPCLOCK_RTCCLK_DIV16, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
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
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LD3_Pin */
  GPIO_InitStruct.Pin = LD3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD3_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
int execute_command(uint8_t * line) {
  uint8_t *cmd;
  uint8_t *arg;
  command_t *p = commands;
  int success = 1;

  if (!line) {
    return (-1); // Passed a bad pointer
  }
  if (parse_command(line,&cmd,&arg) == -1) {
    printf("Error with parse command\n\r");
    return (-1);
  }
  while (p->cmd_string) {
    if (!strcmp(p->cmd_string,(char *) cmd)) {
      if (!p->cmd_function) {
        return (-1);
      }
      success = (*p->cmd_function)((char *)arg);            // Run the command with the passed arguments
      //success = 1;
      break;
    }
    p++;
  }
  if (success == 0) {
    return (0);
  }
  else {
    return (-1);
  }
  }
int parse_command (uint8_t *line, uint8_t **command, uint8_t **args){

  uint8_t *line_temp;
  if((!line) || (!command) || (!args)){ //check for bad pointer
    return(-1);
  }
  line_temp= line;
  *command = line;
  //strcpy(line_left, line);
  while(*line_temp != ','){ //increment through input to find end of command
    if(!*line_temp){
      *args = '\0'; //no arguments
      return(0);
    }
    line_temp++;
  }

  *line_temp = '\0'; //replace comma with null for end of command string
  *args = line_temp +1; //one char past null is beginning of arguments
  return 0;
}
int help_command(char *args){
  printf("\r\nAvailable Commands:\n\r");
  printf("lon\n\r");
  printf("lof\n\r");
  printf("test\n\r");
  printf("ds\n\r");
  return 0;
}
int lon_command(){
  if(led_on == 0){
    HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
    led_on = 1;
    printf("\r\nLED ON\n\r");
  }else{
    printf("\r\nLED ALREADY ON\n\r");
  }
  return 0;
}
int lof_command(){
  if(led_on == 1){
    HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
    led_on = 0;
    printf("\r\nLED OFF\n\r");
  }else{
    printf("\r\nLED ALREADY OFF\n\r");
  }
  return 0;
}
int test_command(char *args){
  printf("\r\ntest\n\r");
  while(*args!='\0'){
    if(*args!=','){
      putchar(*args);
    }else{
      printf("\r\n");
    }
    args++;
  }
  return 0;
}
void printDateTime(){
	RTC_DateTypeDef date;
	RTC_TimeTypeDef time;
	HAL_RTC_GetTime(&hrtc,&time,RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc,&date,RTC_FORMAT_BIN);
	printf("%2.2u/%2.2u/%4.4u %2.2u:%2.2u:%2.2u",date.Month,date.Date,date.Year+2000,time.Hours,time.Minutes,time.Seconds);
}
int dateSet_command(char *args){
	RTC_DateTypeDef date;
	//HAL_StatusTypeDef hal;
	int comma_count = 0;
	char* start_of_num = args;
	int numbers[3];
	while(*args!='\0'){
	    if(*args==','){
	    	*args = '\0';
	        numbers[comma_count] = atoi(start_of_num);
	        comma_count += 1;
	        start_of_num = args+1;
	    }
	    if(comma_count>2){
	    	//printf("\r\n error \n\r");
	    	return 1;
	    }
	    args++;
	  }
	numbers[comma_count] = atoi(start_of_num);
	if(numbers[0]>12 || numbers[0] < 1 || numbers[1] > 31 || numbers[1]<1 || numbers[2] > 100 || numbers[2]<0)
		return 1;
	date.WeekDay = 1;
	date.Month = numbers[0];
	date.Date = numbers[1];
	date.Year = numbers[2];
	HAL_RTC_SetDate(&hrtc,&date,RTC_FORMAT_BIN);
	return 0;
	//hal = HAL_RTC_SetDate(&hrtc,&date,RTC_FORMAT_BIN);
	//printf("\r\n%x\r\n",hal);
}
int timeSet_command(char *args){
	RTC_TimeTypeDef time;
	int comma_count = 0;
	char* start_of_num = args;
	int numbers[3];
	while(*args!='\0'){
		if(*args==','){
			*args = '\0';
		    numbers[comma_count] = atoi(start_of_num);
		    comma_count += 1;
		    start_of_num = args+1;
		}
		if(comma_count>2){
			//printf("\r\n error \n\r");
		    return 1;
		}
		args++;
	}
	if(numbers[0]>24 || numbers[0] < 1 || numbers[1] > 60 || numbers[1]<1 || numbers[2] > 60 || numbers[2]< 1)
		return 1;
	numbers[comma_count] = atoi(start_of_num);
	time.Hours = numbers[0];
	time.Minutes = numbers[1];
	time.Seconds = numbers[2];

	HAL_RTC_SetTime(&hrtc,&time,RTC_FORMAT_BIN);
	return 0;
}
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	toggle=1;
	//HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
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

