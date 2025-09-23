/*
 *	exti.c
 *	Created on: Sep 23, 2025
 *		Author: Trần Minh Đức
 */

#include "gpio.h"
#include "exti.h"
#include "system_config.h"

static ExtiCallback_t exti_callback[MAX_EXTI_LINES];

void Exti_Init(GpioX_t exti_io, ExtiTrigger_t trigger, ExtiCallback_t fuc_cb)
{
    uint32_t pin_number = __builtin_ctz(exti_io.pin);

    GpioX_InInit(exti_io, GPIO_MODE_INPUT); //Enable clock GPIO
}