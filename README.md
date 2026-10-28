# FreeRTOS

## 第一节：初识FreeRTOS——CubeMX配置 API函数 任务创建与运行
　　在了解基本的嵌入式外设后，学习FreeRTOS可以说是水到渠成的事情，其本质上就是将 GPIO， ADC，定时器，有线通信等均视为**任务**，并提供统一的函数接口及调度管理方式。

　　CubeMX 配置中，最上面四个是最为常用的外设部分，其对应的基本开发想必已经信手拈来了。FreeRTOS 的配置位于**Middleware and Software Packs**，如下图。
  
  <img width="360" height="447" alt="屏幕截图 2026-10-28 153018" src="https://github.com/user-attachments/assets/2ce947b3-246a-4830-96f2-48dd0d1d7603" />

　　需要修改的地方主要在于**TICK_RATE_HZ**/**Memory management settings（定义栈大小）**/**Hook function related definitions（异常检测钩子函数）**/**Run time and tasks stat gathering**/**Software Timers**

　　CubeMX 生成代码后，即可尝试简单任务代码：两 LED 交替闪烁 + 串口 共学习如下六个 API 函数:
  
- **xTaskCreate** 创建任务
- **xTaskGetTickCount** 获取时刻（基于SysTick）
- **vTaskDelay** 相对延时
- **vTaskDelayUntil** 绝对延时
- **vTaskDelete** 删除任务
- **vTaskList** 列出任务

　　其中 x 前缀意味着有返回值，v 意味着没有（即 void）。注意！！！**函数不都是可以直接用的，若出现报错需要在*FreeRTOSConfig.h*中及时查看对应函数开启宏定义是否为1**。

　　接下来对函数用法作简要总结：

    

  
