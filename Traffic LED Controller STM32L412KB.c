/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Traffic Light System with Pedestrian Button
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private define ------------------------------------------------------------*/
#define GreenLED_ON()     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET)
#define GreenLED_OFF()    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET)

#define YellowLED_ON()    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET)
#define YellowLED_OFF()   HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET)

#define RedLED_ON()       HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET)
#define RedLED_OFF()      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET)

#define Button_Read()     HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3)

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);

/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

int main(void)
{
    /* Initialize HAL and system clock */
    HAL_Init();
    SystemClock_Config();

    /* Initialize GPIOs */
    MX_GPIO_Init();

    /* Infinite loop */
    while (1)
    {
        // Check button state with simple debounce
        if (Button_Read() == GPIO_PIN_RESET)  // Button pressed
        {
            HAL_Delay(50); // Debounce delay
            if (Button_Read() == GPIO_PIN_RESET)
            {
                while (Button_Read() == GPIO_PIN_RESET); // Wait until button release

                // Pedestrian request sequence
                GreenLED_OFF();

                YellowLED_ON();
                HAL_Delay(2000); // 2 seconds
                YellowLED_OFF();

                RedLED_ON();
                HAL_Delay(5000); // 5 seconds
                RedLED_OFF();
            }
        }
        else
        {
            // Default traffic cycle
            GreenLED_ON();
            HAL_Delay(5000); // 5 seconds
            GreenLED_OFF();

            YellowLED_ON();
            HAL_Delay(2000); // 2 seconds
            YellowLED_OFF();

            RedLED_ON();
            HAL_Delay(5000); // 5 seconds
            RedLED_OFF();
        }
    }
}

/* System Clock Configuration */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
    RCC_OscInitStruct.MSIState = RCC_MSI_ON;
    RCC_OscInitStruct.MSICalibrationValue = 0;
    RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
    RCC_OscInitStruct.PLL.PLLM = 1;
    RCC_OscInitStruct.PLL.PLLN = 40;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);
}

/* GPIO Initialization */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* Configure LED pins */
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Configure Button pin */
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Reset all LEDs */
    GreenLED_OFF();
    YellowLED_OFF();
    RedLED_OFF();
}

/* Error Handler */
void Error_Handler(void)
{
    __disable_irq();
    while (1) {}
}
