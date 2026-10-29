#include "user_task.h"

// 队列句柄
QueueHandle_t q_raw;    // q_raw：A原始数据 → B滤波器
QueueHandle_t q_filter; // q_filter：B滤波输出 → C‑PID

// TaskA：采集原始数据（比如ADC采样）
void TaskA(void *pvParameters)
{
    uint16_t adc_raw = 0;
    while(1)
    {
        adc_raw++;          //模拟ADC采集原始值
        xQueueSend(q_raw, &adc_raw, pdMS_TO_TICKS(10));
        
        vTaskDelay(pdMS_TO_TICKS(100)); //模拟采样周期100ms
    }
}

// TaskB：数字滤波器，接收原始数据，滤波运算
void TaskB(void *pvParameters)
{
    uint16_t raw_val;
    uint16_t filter_out;
    while(1)
    {
        //阻塞等待A送来原始采样数据
        xQueueReceive(q_raw, &raw_val, portMAX_DELAY);
        
        filter_out = raw_val * 0.8;  //模拟滤波算法
        
        //滤波完成，送给PID任务C
        xQueueSend(q_filter, &filter_out, pdMS_TO_TICKS(10));
    }
}

// TaskC：PID控制器，接收滤波之后的数据
void TaskC(void *pvParameters)
{
    uint16_t fb_val;
    while(1)
    {
        xQueueReceive(q_filter, &fb_val, portMAX_DELAY);

        if (fb_val > 40 && fb_val < 80)
        {
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET); // LED ON
        }
        else HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);  // LED OFF
        //拿到经过滤波后的反馈值，执行PID计算
    }
}

void Init(void)
{
  //【原则】先创建队列，后创建任务
  q_raw = xQueueCreate(4, sizeof(uint16_t));
  q_filter = xQueueCreate(4, sizeof(uint16_t));

  xTaskCreate(TaskA,"TaskA",128,NULL,2,NULL);
  xTaskCreate(TaskB,"TaskB",128,NULL,2,NULL);
  xTaskCreate(TaskC,"TaskC",128,NULL,2,NULL);
}

/*static void Task_LED1(void *arg);
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
}*/

