#ifndef _CPU_TASK_H
#define _CPU_TASK_H

#define CPU_INFO_LIST_LENGTH 400

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"

void CPU_task(void *pvParameters);

#endif // !_CPU_TASK_H
