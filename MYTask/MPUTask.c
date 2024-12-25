#include "MPUTask.h"

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"







void mpu6050_task(void *argument)
{
  /* USER CODE BEGIN mpu6050_task */
		
		
  /* Infinite loop */
  for(;;)
  {


    osDelay(1);
  }
  /* USER CODE END mpu6050_task */
}