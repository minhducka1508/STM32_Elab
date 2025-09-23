/*
 *  pwmx.h
 *  Created on: Sep 23, 2025
 *      Author: Trần Minh Đức
 */

#ifndef PWMX_H
#define PWMX_H

// #include "stm32f1xx_hal.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct
{
    TIM_TypeDef *tim;  // TIM2, TIM3, ...
    uint32_t channel;  // TIM_CHANNEL_1..4
    uint32_t freq_hz;  // tần số PWM
    uint32_t duty_pct; // phần trăm duty (0-100)
} PwmX_t;

void PwmX_Init(PwmX_t *pwm, TIM_TypeDef *tim, uint32_t channel, uint32_t freq_hz, uint32_t duty_pct);
void PwmX_Start(PwmX_t *pwm);
void PwmX_Stop(PwmX_t *pwm);
void PwmX_SetDuty(PwmX_t *pwm, uint32_t duty_pct);

#endif
