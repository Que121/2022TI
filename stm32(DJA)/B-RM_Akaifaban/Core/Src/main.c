/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
	已经完成板载所有led移植，串口按需要移植
	printf已经重映射到串口7
	
	
	注意BUG！DMA初始化一定要放到UART初始化前面
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "can.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "MyConfig.h"						//包含所有可配置的东东！

#include "myUART.h"
#include "superCap.h"
#include "led.h"
#include "key.h"
#include "DR16.h"
#include "myMPU6500.h"
#include "buzzer.h"
#include "IMU.h"
#include "bsp_can.h"
#include "motor.h"
#include "power.h"
#include "VisionUART.h"
#include "PYchassis.h"
#include "MyPWM.h"
#include "PYlift.h"
#include "CPXT.h"

#include "PY_2022DianSai.h"
#include "HYQY_RECV.h"
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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
double angle_Temp = 0;
uint8_t angle_Temp_IS_OK = 0;
int16_t tempAngle = 0;
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

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_UART7_Init();
  MX_UART8_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_USART6_UART_Init();
  MX_SPI5_Init();
  MX_TIM12_Init();
  MX_CAN1_Init();
  MX_CAN2_Init();
  MX_TIM1_Init();
  MX_TIM5_Init();
  MX_TIM2_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
	DianSai2022_Init();
//	My_UART_Init();					//串口接收初始化，包括裁判系统接收
//	printf("test printf %d\n",666);
//	DR16_Init();						//初始化遥控器接收机
	//MPU6500_Init();				//初始化MPU
	//IMU_Init();	        	//初始化滤波器
//	buzzer_Init();					//初始化蜂鸣器
	//buzzerOn(50);					//测试蜂鸣器
	//myCanInit_ALL();				//配置底层can相关功能，包括收发
//	MOTOR_Init();						//初始化电机PID相关参数 
	
	
	//power_on_24v_ALL();			//依次上电
	//power_on_24v(3);
	//power_on_24v(1);
	//MyPWM_Init();
	//TIM2->CCR1 = 500;
	//while(1);
	
//	PY_Lift_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	
	angle_Temp += 10;
	SetLiftMotorByPID_Angle(angle_Temp,0,0,0);
		
  while (1)
  {
		Dian2022Ctrl_Handle();
//		if(remote_control.switch_right == Switch_Middle)
//		{
//			SetLiftMotorByPID_Angle(180,180,0,0);
//			//LiftMotor[0].PID_speed.target = 500;
//			//SetLeftMotorRAW(2000,0,0,0);
//		}
//		else
//		{
//			SetLiftMotorByPID_Angle(0,0,0,0);
//			//SetLeftMotorRAW(0,0,0,0);
//			LiftMotor[0].PID_speed.target = 0;
//		}
		//LiftMOTOR_PID_Control_Angle();
		//printf("RM:%.2f,%.2f,%.2f,%.2f,%.2f\n",LiftMotor[0].PID_speed.err,LiftMotor[0].PID_speed.pout,LiftMotor[0].PID_speed.iout,LiftMotor[0].PID_position.measure,LiftMotor[0].PID_position.pout);
		//tempAngle = remote_control.ch2/660.0 * 180.0;
		//printf("ang:%f\n", PY_2022.leftAng);
		
		//SetMotorByPID_Speed(remote_control.ch2*20,-remote_control.ch2*20);
		
		//turnServoCtrl(remote_control.ch3/660.0 * 60.0);
		//printf("ang:%.2f\n", remote_control.ch3/660.0 * 90.0);
		
		
		//测试不定长收发
//		if(IsReceiveDataOK(&HYQY_recvInfo[0]))
//            ShowDecodeResult(&HYQY_recvInfo[0]);
		
//		printf("%d, %d, %d\n", HYQY_recvInfo[0].recvRes[0],HYQY_recvInfo[0].recvRes[1], HYQY_recvInfo[0].recvRes[2]);
		HAL_Delay(5);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 6;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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
	printf("error !!!\nfile:");
	printf(file);
	printf("\nline:%d\n",line);
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
