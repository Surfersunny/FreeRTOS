# FreeRTOS

## 第一节：初识FreeRTOS——CubeMX配置 API函数 任务创建与运行
　　在了解基本的嵌入式外设后，学习FreeRTOS可以说是水到渠成的事情，其本质上就是将 GPIO， ADC，定时器，有线通信等均视为**任务**，并提供统一的函数接口及调度管理方式。

　　CubeMX 配置中，最上面四个是最为常用的外设部分，其对应的基本开发想必已经信手拈来了。FreeRTOS 的配置位于**Middleware and Software Packs**，如下图。
  
  <img width="360" height="447" alt="屏幕截图 2026-10-28 153018" src="https://github.com/user-attachments/assets/2ce947b3-246a-4830-96f2-48dd0d1d7603" />

　　按需修改的地方主要在于：**TICK_RATE_HZ（1-1000的整数，决定FreeRTOS执行时间片）Memory management settings（定义栈大小）Hook function related definitions（异常检测钩子函数）Run time and tasks stat gathering（主要相关函数vTaskList任务查看等）Software Timers（软件定时器）**

　　CubeMX 生成代码后，即可尝试简单任务代码：两 LED 交替闪烁 + 串口 共学习如下六个 API 函数:
  
- **xTaskCreate** 创建任务
- **xTaskGetTickCount** 获取时刻（基于SysTick）
- **vTaskDelay** 相对延时
- **vTaskDelayUntil** 绝对延时
- **vTaskDelete** 删除任务
- **vTaskList** 列出任务

　　其中 x 前缀意味着有返回值，v 意味着没有（即 void）。注意！！！**函数不都是可以直接用的，若出现报错需要在*FreeRTOSConfig.h*中及时查看对应函数开启宏定义是否为1**。

　　接下来对函数用法作简要总结：

　　创建任务示例
　　
  xTaskCreate(Task_LED1, "LED1", 128, NULL, 2, &hTaskLED1)

　　**必备函数**！Task_LED1即为任务函数，“LED1”为名称（TaskList显示的即为此名称），128为分配内存空间，“2”为优先级（数字越大优先级越高），hTaskLED1为实际句柄TaskHandle_t（类比对象是类的实例，可以认为是任务函数的实例化）

  vTaskDelay(pdMS_TO_TICKS(300)) vTaskDelayUntil(&now, pdMS_TO_TICKS(1000))

　　从函数签名即可看出相对和绝对延时的区别：相对是从任务完成后才开始计时，必然会受到任务执行耗时的影响，不可用于精确定时，绝对延时记录结束时刻规划下一次执行的精确时间戳。同时注意两个函数都只控制任务在**阻塞&就绪**状态间切换，对于函数**运行**不加以任何干涉！！！
  
  vApplicationMallocFailedHook(void) vApplicationStackOverflowHook(void)

　　两个钩子函数用于异常防范：区别在于Malloc更大（针对整个堆大小而言，heap是cubemx中配置的总空间！FreeRTOS所有任务+变量均消耗此空间，相当于总池子不够） StackOverflow较小（针对某一任务的小池子，相当于分配的小池子不够该任务所需空间，如分配任务共128B然而任务内定义了256B大小的数组）

    

  
