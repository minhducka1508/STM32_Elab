/*
 *	exti.h
 *	Created on: Sep 23, 2025
 *		Author: Trần Minh Đức
 */

#ifndef _EXTI_H_
#define _EXTI_H_

#include "stdint.h"
#include "stdio.h"

typedef enum
{
    EXTI_RISING,
    EXTI_FALLING,
    EXTI_RISING_FALLING
} ExtiTrigger_t;

typedef void (*ExtiCallback_t)(void); // Hàm callback kiểu function pointer

void  Exti_Init(GPIO_TypeDef *port, uint16_t pin, ExtiTrigger_t trigger, ExtiCallback_t fun_cb); // Hàn cấu hình 1 chân EXTI

void Exti_IRQHandler(uint16_t pin); // Hàm được gọi trong IRQ Handler

#endif /* _EXTI_H_ */