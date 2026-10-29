#ifndef __USER_TASK_H
#define __USER_TASK_H

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "gpio.h"
#include "usart.h"

#include "stdio.h"
#include "string.h"

void Init(void);   //任务创建入口

#endif
