#include "stm32f4xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"

#define GREEN_LED_PIN GPIO_PIN_12
#define ORANGE_LED_PIN GPIO_PIN_13
#define RED_LED_PIN GPIO_PIN_14
#define BLUE_LED_PIN GPIO_PIN_15

void GPIO_Init(void){
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOD_CLK_ENABLE();
    GPIO_InitStruct.Pin = GREEN_LED_PIN | ORANGE_LED_PIN | RED_LED_PIN | BLUE_LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}

void TaskGreenLED(void *pvParameters)
{
    while (1)
    {
        HAL_GPIO_TogglePin(GPIOD, GREEN_LED_PIN);
        vTaskDelay(pdMS_TO_TICKS(500)); // Toggle every 500 ms
    }
}

void TaskOrangeLED(void *pvParameters)
{
    while (1)
    {
        HAL_GPIO_TogglePin(GPIOD, ORANGE_LED_PIN);
        vTaskDelay(pdMS_TO_TICKS(500)); // Toggle every 500 ms
    }
}

void TaskRedLED(void *pvParameters)
{
    while (1)
    {
        HAL_GPIO_TogglePin(GPIOD, RED_LED_PIN);
        vTaskDelay(pdMS_TO_TICKS(500)); // Toggle every 500 ms
    }
}

void TaskBlueLED(void *pvParameters)
{
    while (1)
    {
        HAL_GPIO_TogglePin(GPIOD, BLUE_LED_PIN);
        vTaskDelay(pdMS_TO_TICKS(500)); // Toggle every 500 ms
    }
}

int main(void)
{
    HAL_Init();
    GPIO_Init();

    // Create tasks for each LED
    xTaskCreate(TaskGreenLED, "Green LED Task", 128, NULL, 1, NULL);
    xTaskCreate(TaskOrangeLED, "Orange LED Task", 128, NULL, 1, NULL);
    xTaskCreate(TaskRedLED, "Red LED Task", 128, NULL, 1, NULL);
    xTaskCreate(TaskBlueLED, "Blue LED Task", 128, NULL, 1, NULL);

    // Start the scheduler
    vTaskStartScheduler();

    // Infinite loop (should never reach here)
    while (1)
    {
    }
}