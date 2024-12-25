#include "LCDTask.h"
#include "adc.h"

#include "FreeRTOS.h"
#include "task.h"

#include "cmsis_os.h"

#include "tim.h"
#include "usart.h"

#include "lvgl.h"                // 它为整个LVGL提供了更完整的头文件引用
#include "lv_port_disp.h"        // LVGL的显示支持
#include "lv_port_indev.h"       // LVGL的触屏支持

#include "mpu6050.h" //mpu6050驱动程序的头文件

lv_ui guider_ui;

uint8_t rawData[4]={0};

extern uint8_t ID;

void LCDTask(void *argument)
{
  /* USER CODE BEGIN LCDTask */
	  /* Initialize the LCD */
  BSP_LCD_Init();

  /* Layer1 Init */
  BSP_LCD_LayerDefaultInit(0, LCD_FRAME_BUFFER_LAYER0);
  
  /* Set Foreground Layer */
  BSP_LCD_SelectLayer(0);
 
  /* Enable The LCD */
  BSP_LCD_DisplayOn();

  /* Clear the LCD */
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /*##-2- Touch screen initialization ########################################*/
//  Touchscreen_Calibration();
  BSP_TS_Init(240, 320);
  
	HAL_UART_Receive_IT(&huart1,rawData,4);//串口接收中断
	
	HAL_TIM_PWM_Start(&htim9,TIM_CHANNEL_1);//PWM开启
	HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_3);//PWM开启
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);//PWM开启
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);//PWM开启
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);//PWM开启、
	
	MPU6050_Init();		//MPU6050初始化
	ID = MPU6050_GetID();				//获取MPU6050的ID号
	
	lv_init();                             // LVGL 初始化
	lv_port_disp_init();                   // 注册LVGL的显示任务
	lv_port_indev_init();                  // 注册LVGL的触屏检测任务
	
	setup_ui(&guider_ui);
  events_init(&guider_ui);
	

	

  /* Infinite loop */
  for(;;)
  {
		lv_timer_handler();
    osDelay(1);
  }
  /* USER CODE END LCDTask */
}

void vApplicationTickHook( void )//1ms进入一次钩子函数
{
   /* This function will be called by each tick interrupt if
   configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h. User code can be
   added here, but the tick hook is called from an interrupt context, so
   code must not attempt to block, and only the interrupt safe FreeRTOS API
   functions can be used (those that end in FromISR()). */
	
	
	lv_tick_inc(1); // LVGL的1ms中断
}


