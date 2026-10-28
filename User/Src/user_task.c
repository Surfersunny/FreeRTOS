#include "user_task.h"

static void Task_LED1(void *arg);
static void Task_LED2(void *arg);
static void Task_UART(void *arg);

//任务句柄
TaskHandle_t hTaskLED1 = NULL;
TaskHandle_t hTaskLED2 = NULL;
TaskHandle_t hTaskUART = NULL;

char taskListBuf[250];

float x = 0.0f;

void Init(void)
{
    xTaskCreate(Task_LED1, "LED1", 128, NULL, 2, &hTaskLED1);
    xTaskCreate(Task_LED2, "LED2", 128, NULL, 2, &hTaskLED2);
    xTaskCreate(Task_UART, "UART", 256, NULL, 2, &hTaskUART);

    vTaskList(taskListBuf);

    HAL_UART_Transmit_IT(&huart1, (uint8_t *)taskListBuf, strlen(taskListBuf));
}

static void Task_LED1(void *arg)
{
    (void)arg;
    for(;;)
    {
        x = x + 1.0f;
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
        vTaskDelay(pdMS_TO_TICKS(300));
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
        vTaskDelay(pdMS_TO_TICKS(300));

        if (x > 20.0f)
        {
            vTaskDelete(hTaskLED1);
        }
    }
}

static void Task_LED2(void *arg)
{
    (void)arg;
    for(;;)
    {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
        vTaskDelay(pdMS_TO_TICKS(800));
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
        vTaskDelay(pdMS_TO_TICKS(800));
    }
}

static void Task_UART(void *arg) {
    (void)arg;
    
    TickType_t now;
    char buf[32];

    for(;;)
    {
        now = xTaskGetTickCount();
        vTaskDelay(pdMS_TO_TICKS(1000));

        snprintf(buf, sizeof(buf), "Tick: %lu\r\n", (unsigned long)now);
        //HAL_UART_Transmit_IT(&huart1, (uint8_t *)buf, strlen(buf));

        //HAL_UART_Transmit_IT(&huart1, (uint8_t *)"Hello FreeRTOS!\r\n", 17);
    }
}

