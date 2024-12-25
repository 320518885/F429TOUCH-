/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

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
/* USER CODE BEGIN Variables */
// 声明新的定时器句柄
static osTimerId_t thirdTimerId;
static osTimerAttr_t thirdTimer_attributes = {
    .name = "ThirdTimer"
};


// 声明定时器句柄
static osTimerId_t myTimerId;
static osTimerAttr_t timer_attributes = {
    .name = "MyTimer"
};

// 声明另一个定时器句柄
static osTimerId_t anotherTimerId;
static osTimerAttr_t anotherTimer_attributes = {
    .name = "Another1msTimer"
};

/* USER CODE END Variables */
/* Definitions for MYLCDTask */
osThreadId_t MYLCDTaskHandle;
const osThreadAttr_t MYLCDTask_attributes = {
  .name = "MYLCDTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ADCTask */
osThreadId_t ADCTaskHandle;
const osThreadAttr_t ADCTask_attributes = {
  .name = "ADCTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal7,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void timer_callback(void *argument);
void timer_callback_another(void *argument);
void timer_callback_third(void *argument);// 新的定时器回调函数
/* USER CODE END FunctionPrototypes */

void LCDTask(void *argument);
void ADC_task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationTickHook(void);

/* USER CODE BEGIN 3 */

/* USER CODE END 3 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
	  // 创建定时器，定时1ms
		myTimerId = osTimerNew(timer_callback, osTimerPeriodic, NULL, &timer_attributes);
		if (myTimerId != NULL) {
				// 启动定时器，间隔1ms
				osTimerStart(myTimerId, 1);  // 1ms定时
		}
  /* add queues, ... */
		
		// 创建另一个定时器，定时1ms
		anotherTimerId = osTimerNew(timer_callback_another, osTimerPeriodic, NULL, &anotherTimer_attributes);
		if (anotherTimerId != NULL) {
			// 启动另一个定时器，间隔1ms
			osTimerStart(anotherTimerId, 1);  // 1ms定时
		}
		
	/* 创建并启动新定时器 */
  thirdTimerId = osTimerNew(timer_callback_third, osTimerPeriodic, NULL, &thirdTimer_attributes);
  if (thirdTimerId != NULL) {
    osTimerStart(thirdTimerId, 1);  // 1ms定时
  }
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of MYLCDTask */
  MYLCDTaskHandle = osThreadNew(LCDTask, NULL, &MYLCDTask_attributes);

  /* creation of ADCTask */
  ADCTaskHandle = osThreadNew(ADC_task, NULL, &ADCTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */

  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_LCDTask */
/**
  * @brief  Function implementing the MYLCDTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_LCDTask */
__weak void LCDTask(void *argument)
{
  /* USER CODE BEGIN LCDTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END LCDTask */
}

/* USER CODE BEGIN Header_ADC_task */
/**
* @brief Function implementing the ADCTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ADC_task */
__weak void ADC_task(void *argument)
{
  /* USER CODE BEGIN ADC_task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END ADC_task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
// 定时器回调函数，每1毫秒执行一次
__weak void timer_callback(void *argument)
{
  
  
}

// 另一个定时器的回调函数，每1毫秒执行一次
__weak void timer_callback_another(void *argument)
{
  // 定时器回调函数的代码
  // 这里可以放置需要每1毫秒执行一次的代码
}
/* USER CODE END Application */

// 第三个定时器的回调函数，每1毫秒执行一次
__weak void timer_callback_third(void *argument)
{

}