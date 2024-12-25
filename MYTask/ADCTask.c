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
int nextpage = 1;//��ʼ��1Ϊ��ҳ��2Ϊͼ��ҳ

extern double pitch_g;

extern void update_light_bar(int value);//���������º���
extern void update_chart_value(int var_value);//����ͼ������ֵ
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
		
		if(nextpage == 1)update_light_bar(Light_strength_handred);//�ڲ�ͬ��ҳ����²�ͬ��ֵ�������ҳ��1����ҳ��2��ֵ�Ῠ��
		else if(nextpage == 2)update_chart_value(Pwm2_wave);
		else if(nextpage == 3)
		{
			p++;
			
			if(p>500){
				update_chart_value_2(pitch_g);
					p=0;
			}
		}// ѭ���ĴΣ�ÿ�γ���10��ȡ������Ϊ�����һ��Ԫ��
		for (int i = 3; i >= 0; i--) {
				sensorReadings[i] = Light_strength % 10;  // ��ȡ��ǰ���λ
				Light_strength /= 10;             // ���� 10 �ƶ�����һλ
		}
//		update_chart_value_2(1000);
		
		HAL_UART_Transmit(&huart1,sensorReadings,4,HAL_MAX_DELAY);//���͵�ǰ�Ĺ�ǿ
		
    osDelay(1);
  }
  /* USER CODE END ADC_task */
}



