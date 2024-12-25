#include "OtherTask.h"


#include "FreeRTOS.h"
#include "task.h"

#include "tim.h"

#include "cmsis_os.h"

#include "main.h"

#include "usart.h"

#include "mpu6050.h"
#include "kfresolving.h"

uint8_t lightMode  = 1; // 1: 三色模式, 2: 双色模式
uint8_t colorChannel  = 0;         // 当前颜色状态 (0: 通道1, 1: 通道2, 2: 通道3 或 4)
uint16_t brightnessLevel  = 0;              // brightnessLevel 值 (亮度)
uint8_t brightnessDirection  = 0;        // 亮度递增递减标志 (0: 递增, 1: 递减)

uint8_t ID;								//定义用于存放ID号的变量
int16_t AX, AY, AZ, GX, GY, GZ;			//定义用于存放各个MPU数据的变量
	
extern uint8_t rawData[4];//接收数据


void colorchange(void);
void UpdateBreathingLight(void);

extern void update_step_count_label(int step);
extern void update_chart_value2(int var_value);

// 定时器回调函数，每1毫秒执行一次
void timer_callback(void *argument)
{
	
  UpdateBreathingLight();//呼吸灯程序
  
}

// 定时器回调函数，每1毫秒执行一次
void timer_callback_another(void *argument)
{
  	//计步程序
	if(lightMode == 4){//因为卡尔曼计算量有点大，为了避免影响呼吸灯所以在mode为4的时候再计步
		MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);		//获取MPU6050的数据
		kf();
	}

}




//串口中断回调函数
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */
	
	if(lightMode  == 3)
	{
		
		int pulseValue = 0;
		for (int i = 0; i < 4; i++) {
				pulseValue = (pulseValue << 3) + (pulseValue << 1) + rawData[i];  // 乘法换算得到千位数
		}
		__HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_3, pulseValue); //更新亮度
		__HAL_TIM_SetCompare(&htim9, TIM_CHANNEL_1, pulseValue); 
	}
	
	HAL_UART_Receive_IT(&huart1,rawData,4);
  /* USER CODE END USART1_IRQn 1 */
}



/**
  * @brief This function handles EXTI line[15:10] interrupts.
  */
void EXTI15_10_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI15_10_IRQn 0 */

  /* USER CODE END EXTI15_10_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
  /* USER CODE BEGIN EXTI15_10_IRQn 1 */
	colorchange();
  /* USER CODE END EXTI15_10_IRQn 1 */
}


void colorchange(void)
{
    if (lightMode  == 1) { // 当前是双色模式
        lightMode  = 2;     // 切换为三色模式
        colorChannel  = 0;    // 重新初始化颜色
        brightnessDirection  = 0;   // 亮度递增模式
        brightnessLevel  = 0;      // 初始化brightnessLevel 值

        // 初始化定时器通道
        __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2, 0);
        __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, 0);
        __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_2, 0);
    } 
    else if (lightMode  == 2) { // 当前是三色模式
        lightMode  = 3;     // 切换为双色模式
        colorChannel  = 0;    // 重新初始化颜色
        brightnessDirection  = 0;   // 亮度递增模式
        brightnessLevel  = 0;      // 初始化brightnessLevel 值

        // 初始化定时器通道
        __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_3, 0);
        __HAL_TIM_SetCompare(&htim9, TIM_CHANNEL_1, 0);
    } 
    else if (lightMode  == 3) { // 当前是串口模式
        lightMode  = 4;     // 切换为双色模式
        // 初始化定时器通道
        __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_3, 0);
        __HAL_TIM_SetCompare(&htim9, TIM_CHANNEL_1, 0);
			
    }else if (lightMode  == 4) { // 当前是LCD模式
			
        lightMode  = 1;     // 切换为三色模式
        __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_3, 0);
        __HAL_TIM_SetCompare(&htim9, TIM_CHANNEL_1, 0);
    }
}




void UpdateBreathingLight(void)
{
    // 定义定时器和通道
    TIM_HandleTypeDef* htim[3];  // 使用数组来存储定时器句柄
    uint32_t channels[3];         // 使用数组来存储通道

    // 根据lightMode 选择定时器和通道
    if (lightMode  == 1) { // 三色模式, 使用三个定时器
        // 为lightMode  1选择定时器和通道
        htim[0] = &htim2;  // 使用htim2通道2
        htim[1] = &htim3;  // 使用htim3通道1
        htim[2] = &htim4;  // 使用htim4通道2
        channels[0] = TIM_CHANNEL_2;
        channels[1] = TIM_CHANNEL_1;
        channels[2] = TIM_CHANNEL_2;  // htim4的通道2
    } 
    else if (lightMode  == 2) { // 双色模式, 使用两个定时器
        // 为lightMode  2选择定时器和通道
        htim[0] = &htim8;  // 使用htim8通道3
        htim[1] = &htim9;  // 使用htim9通道1
        channels[0] = TIM_CHANNEL_3;
        channels[1] = TIM_CHANNEL_1;
    } 
    else {
        return; // 如果状态不符合，退出函数
    }

    // 设置brightnessLevel 值
    for (int i = 0; i < (lightMode  == 1 ? 3 : 2); i++) {
        // 如果是lightMode  2，则设置brightnessLevel 值为2 * brightnessLevel 
        if (lightMode  == 2) {
            __HAL_TIM_SetCompare(htim[i], channels[i], 2 * brightnessLevel ); 
        } else {
            __HAL_TIM_SetCompare(htim[i], channels[i], brightnessLevel ); 
        }
    }

    // 亮度递增或递减的逻辑
    if (brightnessDirection  == 0) {
        if (brightnessLevel  < 1000) {
            brightnessLevel ++; // 递增
        } else {
            brightnessDirection  = 1; // 达到最大值，开始递减
        }
    } else {
        if (brightnessLevel  > 0) {
            brightnessLevel --; // 递减
        } else {
            brightnessDirection  = 0; // 达到最小值，开始递增
            // 切换到下一个颜色
            colorChannel  = (colorChannel  + 1) % (lightMode  == 1 ? 3 : 2); // 根据模式切换颜色
        }
    }
}
