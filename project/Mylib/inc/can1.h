#ifndef __CAN1_H__
#define __CAN1_H__

#include <stm32f4xx.h>
#include <stm32f4xx_conf.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "can_receive.h"

#include "debug.h"
#include "chassis_test.h"

/*  params */

#ifdef NO_GIMBAL_TEST // 使用云台板测试情况下
#define CAN1_RCC_AHBx_GPIOx RCC_AHB1Periph_GPIOB
#define CAN1_GPIOx GPIOB
#define CAN1_GPIO_PinSource_x1 GPIO_PinSource8
#define CAN1_GPIO_PinSource_x2 GPIO_PinSource9
#define CAN1_GPIO_Pin_x1 GPIO_Pin_8
#define CAN1_GPIO_Pin_x2 GPIO_Pin_9
#else
#define CAN1_RCC_AHBx_GPIOx RCC_AHB1Periph_GPIOA
#define CAN1_GPIOx GPIOA
#define CAN1_GPIO_PinSource_x1 GPIO_PinSource11
#define CAN1_GPIO_PinSource_x2 GPIO_PinSource12
#define CAN1_GPIO_Pin_x1 GPIO_Pin_11
#define CAN1_GPIO_Pin_x2 GPIO_Pin_12
#endif

/* GPIO特性设置到特定的位置修改 */
/* NVIC优先级到特定位置修改 */
/* CAN1特性配置到特定位置修改 */
/* 过滤器的其它设置到特定位置改 */

void CAN1_Configuration(void);

#endif
