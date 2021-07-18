/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#include "oled.h"
#include "stdio.h"
#include "math.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern uint32_t AD_TwoChanel_value[2];
extern uint8_t OLED_1_data[8][128];
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId YKC_OLEDHandle;
osThreadId HJY_OLEDHandle;
osThreadId Task_OLED_refreHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartTask02(void const * argument);
void StartTask_OLED_refresh(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

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
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of YKC_OLED */
  osThreadDef(YKC_OLED, StartDefaultTask, osPriorityNormal, 0, 128);
  YKC_OLEDHandle = osThreadCreate(osThread(YKC_OLED), NULL);

  /* definition and creation of HJY_OLED */
  osThreadDef(HJY_OLED, StartTask02, osPriorityIdle, 0, 128);
  HJY_OLEDHandle = osThreadCreate(osThread(HJY_OLED), NULL);


 /* definition and creation of Task_OLED_refre */
  osThreadDef(Task_OLED_refre, StartTask_OLED_refresh, osPriorityLow, 0, 128);
  Task_OLED_refreHandle = osThreadCreate(osThread(Task_OLED_refre), NULL);


  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

void OLED_ShowPixel(uint8_t x,uint8_t y)
{
	OLED_1_data[y/8][x]|=0x01<<(y%8);
}

//x0负责上下，y0负责左右
void draw_circle(uint8_t x0,uint8_t y0,uint8_t r) //圆心(x0,y0),半径r
{
		uint8_t x = 0,y = 0,R = 0;
		for(x = x0-r;x <= x0+r;x++){
			for(y = y0-r; y <= y0+r ;y++ ){
				R = sqrt(pow(r,2)-pow(x-x0,2))+y0; //圆方程  x,y反置		
				if( (y >= y0 && y <= R) || (y < y0 && y >= 2*y0-R )) {  //点限制在 圆方程内	
					OLED_ShowPixel(y,x);
				}	
			}
		}

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the YKC_OLED thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
//	for(uint8_t x = 32;x <= 64;x++){
//	OLED_Clear(OLED_1 );
	draw_circle(32,64,32);
	OLED_ShowString(OLED_1,6,4,"world of wonder",8);
	OLED_Refreash();//40hz屏幕刷新	
//	}
  }

  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief Function implementing the HJY_OLED thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTask02 */
void StartTask02(void const * argument)
{
  /* USER CODE BEGIN StartTask02 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(100);
    HAL_GPIO_TogglePin(GPIOA ,GPIO_PIN_3);
    uint8_t buff[]="";
    snprintf((char*)buff,60,"%d",AD_TwoChanel_value[0]);
    OLED_Clear(OLED_2);
    OLED_ShowString(OLED_2,0,3,(char*)buff,16);
	OLED_Refreash();//40hz屏幕刷新
  }
  /* USER CODE END StartTask02 */
}

void StartTask_OLED_refresh(void const * argument)
{
  /* USER CODE BEGIN StartTask_OLED_refresh */
  /* Infinite loop */
  for(;;)
  {
    osDelay(25);
    OLED_Refreash();//40hz屏幕刷新
  }
  /* USER CODE END StartTask_OLED_refresh */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
