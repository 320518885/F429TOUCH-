#include "OtherTask.h"


#include "FreeRTOS.h"
#include "task.h"

#include "tim.h"

#include "cmsis_os.h"

#include "main.h"

#include "usart.h"

#include "mpu6050.h"
#include "kfresolving.h"

uint8_t lightMode  = 1; // 1: ��ɫģʽ, 2: ˫ɫģʽ
uint8_t colorChannel  = 0;         // ��ǰ��ɫ״̬ (0: ͨ��1, 1: ͨ��2, 2: ͨ��3 �� 4)
uint16_t brightnessLevel  = 0;              // brightnessLevel ֵ (����)
uint8_t brightnessDirection  = 0;        // ���ȵ����ݼ���־ (0: ����, 1: �ݼ�)

uint8_t ID;								//�������ڴ��ID�ŵı���
int16_t AX, AY, AZ, GX, GY, GZ;			//�������ڴ�Ÿ���MPU���ݵı���
	
extern uint8_t rawData[4];//��������


void colorchange(void);
void UpdateBreathingLight(void);

extern void update_step_count_label(int step);
extern void update_chart_value2(int var_value);

// ��ʱ���ص�������ÿ1����ִ��һ��
void timer_callback(void *argument)
{
	
  UpdateBreathingLight();//�����Ƴ���
  
}

// ��ʱ���ص�������ÿ1����ִ��һ��
void timer_callback_another(void *argument)
{
  	//�Ʋ�����
	if(lightMode == 4){//��Ϊ�������������е��Ϊ�˱���Ӱ�������������modeΪ4��ʱ���ټƲ�
		MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);		//��ȡMPU6050������
		kf();
	}

}




//�����жϻص�����
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
				pulseValue = (pulseValue << 3) + (pulseValue << 1) + rawData[i];  // �˷�����õ�ǧλ��
		}
		__HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_3, pulseValue); //��������
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
    if (lightMode  == 1) { // ��ǰ��˫ɫģʽ
        lightMode  = 2;     // �л�Ϊ��ɫģʽ
        colorChannel  = 0;    // ���³�ʼ����ɫ
        brightnessDirection  = 0;   // ���ȵ���ģʽ
        brightnessLevel  = 0;      // ��ʼ��brightnessLevel ֵ

        // ��ʼ����ʱ��ͨ��
        __HAL_TIM_SetCompare(&htim2, TIM_CHANNEL_2, 0);
        __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, 0);
        __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_2, 0);
    } 
    else if (lightMode  == 2) { // ��ǰ����ɫģʽ
        lightMode  = 3;     // �л�Ϊ˫ɫģʽ
        colorChannel  = 0;    // ���³�ʼ����ɫ
        brightnessDirection  = 0;   // ���ȵ���ģʽ
        brightnessLevel  = 0;      // ��ʼ��brightnessLevel ֵ

        // ��ʼ����ʱ��ͨ��
        __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_3, 0);
        __HAL_TIM_SetCompare(&htim9, TIM_CHANNEL_1, 0);
    } 
    else if (lightMode  == 3) { // ��ǰ�Ǵ���ģʽ
        lightMode  = 4;     // �л�Ϊ˫ɫģʽ
        // ��ʼ����ʱ��ͨ��
        __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_3, 0);
        __HAL_TIM_SetCompare(&htim9, TIM_CHANNEL_1, 0);
			
    }else if (lightMode  == 4) { // ��ǰ��LCDģʽ
			
        lightMode  = 1;     // �л�Ϊ��ɫģʽ
        __HAL_TIM_SetCompare(&htim8, TIM_CHANNEL_3, 0);
        __HAL_TIM_SetCompare(&htim9, TIM_CHANNEL_1, 0);
    }
}




void UpdateBreathingLight(void)
{
    // ���嶨ʱ����ͨ��
    TIM_HandleTypeDef* htim[3];  // ʹ���������洢��ʱ�����
    uint32_t channels[3];         // ʹ���������洢ͨ��

    // ����lightMode ѡ��ʱ����ͨ��
    if (lightMode  == 1) { // ��ɫģʽ, ʹ��������ʱ��
        // ΪlightMode  1ѡ��ʱ����ͨ��
        htim[0] = &htim2;  // ʹ��htim2ͨ��2
        htim[1] = &htim3;  // ʹ��htim3ͨ��1
        htim[2] = &htim4;  // ʹ��htim4ͨ��2
        channels[0] = TIM_CHANNEL_2;
        channels[1] = TIM_CHANNEL_1;
        channels[2] = TIM_CHANNEL_2;  // htim4��ͨ��2
    } 
    else if (lightMode  == 2) { // ˫ɫģʽ, ʹ��������ʱ��
        // ΪlightMode  2ѡ��ʱ����ͨ��
        htim[0] = &htim8;  // ʹ��htim8ͨ��3
        htim[1] = &htim9;  // ʹ��htim9ͨ��1
        channels[0] = TIM_CHANNEL_3;
        channels[1] = TIM_CHANNEL_1;
    } 
    else {
        return; // ���״̬�����ϣ��˳�����
    }

    // ����brightnessLevel ֵ
    for (int i = 0; i < (lightMode  == 1 ? 3 : 2); i++) {
        // �����lightMode  2��������brightnessLevel ֵΪ2 * brightnessLevel 
        if (lightMode  == 2) {
            __HAL_TIM_SetCompare(htim[i], channels[i], 2 * brightnessLevel ); 
        } else {
            __HAL_TIM_SetCompare(htim[i], channels[i], brightnessLevel ); 
        }
    }

    // ���ȵ�����ݼ����߼�
    if (brightnessDirection  == 0) {
        if (brightnessLevel  < 1000) {
            brightnessLevel ++; // ����
        } else {
            brightnessDirection  = 1; // �ﵽ���ֵ����ʼ�ݼ�
        }
    } else {
        if (brightnessLevel  > 0) {
            brightnessLevel --; // �ݼ�
        } else {
            brightnessDirection  = 0; // �ﵽ��Сֵ����ʼ����
            // �л�����һ����ɫ
            colorChannel  = (colorChannel  + 1) % (lightMode  == 1 ? 3 : 2); // ����ģʽ�л���ɫ
        }
    }
}
