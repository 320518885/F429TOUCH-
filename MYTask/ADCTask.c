#include "ADCTask.h"

#include "adc.h"

#include "FreeRTOS.h"
#include "task.h"

#include "cmsis_os.h"

#include "usart.h"

#include "main.h"

int Pwm2_wave = 0;
int Light_strength = 0;
int Light_strength_handred = 0;
int nextpage = 1;//初始化1为首页，2为图表页

extern double pitch_g;

extern void update_light_bar(int value);//亮度条更新函数
extern void update_chart_value(int var_value);//更新图表曲线值
extern void update_chart_value_2(int var_value);

int p=0;

uint8_t sensorReadings[4]={0};

void ADC_task(void *argument)
{
  /* USER CODE BEGIN ADC_task */
  /* Infinite loop */
  for(;;)
  {
		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1,HAL_MAX_DELAY);
		Light_strength = HAL_ADC_GetValue(&hadc1);
		
		HAL_ADC_Start(&hadc2);
		HAL_ADC_PollForConversion(&hadc2,HAL_MAX_DELAY);
		Pwm2_wave = HAL_ADC_GetValue(&hadc2);
		
		if(Pwm2_wave< 0)Pwm2_wave=0;
		else if(Pwm2_wave> 2000)Pwm2_wave=3000;
		
		if(Light_strength< 0)Light_strength=0;
		else if(Light_strength> 3000)Light_strength=3000;
		
		Light_strength_handred = (3000 - Light_strength)*100/2700;
		
		if(nextpage == 1)update_light_bar(Light_strength_handred);//在不同的页面更新不同的值，如果在页面1更新页面2的值会卡死
		else if(nextpage == 2)update_chart_value(Pwm2_wave);
		else if(nextpage == 3)
		{
			p++;
			
			if(p>500){
				update_chart_value_2(pitch_g);
					p=0;
			}
		}// 循环四次，每次除以10，取余数作为数组的一个元素
		for (int i = 3; i >= 0; i--) {
				sensorReadings[i] = Light_strength % 10;  // 获取当前最低位
				Light_strength /= 10;             // 除以 10 移动到下一位
		}
//		update_chart_value_2(1000);
		
		HAL_UART_Transmit(&huart1,sensorReadings,4,HAL_MAX_DELAY);//发送当前的光强
		
    osDelay(1);
  }
  /* USER CODE END ADC_task */
}



