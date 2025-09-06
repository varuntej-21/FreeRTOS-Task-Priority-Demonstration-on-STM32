/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : FreeRTOS Scheduling Demo (2 Tasks)
  ******************************************************************************
  * Behavior:
  *  - If Task1 priority > Task2 priority → Only PA2 LED blinks, PA3 OFF
  *  - If Task2 priority > Task1 priority → Only PA3 LED blinks, PA2 OFF
  *  - If Task1 priority == Task2 priority → Both LEDs blink (time-slicing)
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Task handles */
osThreadId_t Task1Handle;
osThreadId_t Task2Handle;

/* Task attributes (EDIT PRIORITIES HERE) */
const osThreadAttr_t Task1_attributes = {
  .name = "Task1",
  .stack_size = 128 * 4,
  .priority = osPriorityHigh   // try osPriorityHigh, osPriorityNormal, osPriorityLow
};
const osThreadAttr_t Task2_attributes = {
  .name = "Task2",
  .stack_size = 128 * 4,
  .priority = osPriorityNormal
};

/* Prototypes */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void Task1_Function(void *argument);
void Task2_Function(void *argument);

/* Main ----------------------------------------------------------------------*/
int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();

  osKernelInitialize();

  /* Create tasks */
  Task1Handle = osThreadNew(Task1_Function, NULL, &Task1_attributes);
  Task2Handle = osThreadNew(Task2_Function, NULL, &Task2_attributes);

  osKernelStart();

  while (1) {}
}

/* Task1: LED on PA2 */
void Task1_Function(void *argument)
{
  for (;;)
  {
    osPriority_t p1 = osThreadGetPriority(Task1Handle);
    osPriority_t p2 = osThreadGetPriority(Task2Handle);

    if (p1 > p2)
    {
      /* Task1 dominates: suspend Task2, keep PA3 OFF */
      osThreadSuspend(Task2Handle);
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);

      HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_2);
      osDelay(500);
    }
    else if (p1 < p2)
    {
      /* Task1 is lower: keep PA2 OFF */
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
      osDelay(50);
    }
    else
    {
      /* Equal priority: both run */
      osThreadResume(Task2Handle);
      HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_2);
      osDelay(500);
    }
  }
}

/* Task2: LED on PA3 */
void Task2_Function(void *argument)
{
  for (;;)
  {
    osPriority_t p1 = osThreadGetPriority(Task1Handle);
    osPriority_t p2 = osThreadGetPriority(Task2Handle);

    if (p2 > p1)
    {
      /* Task2 dominates: suspend Task1, keep PA2 OFF */
      osThreadSuspend(Task1Handle);
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);

      HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_3);
      osDelay(500);
    }
    else if (p2 < p1)
    {
      /* Task2 is lower: keep PA3 OFF */
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
      osDelay(50);
    }
    else
    {
      /* Equal priority: both run */
      osThreadResume(Task1Handle);
      HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_3);
      osDelay(500);
    }
  }
}

/* System Clock Configuration */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3);
}

/* GPIO Init */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  __HAL_RCC_GPIOA_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void Error_Handler(void)
{
  __disable_irq();
  while (1) {}
}
