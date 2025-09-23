/*
 *  pwmx.c
 *  Created on: Sep 23, 2025
 *      Author: Trần Minh Đức
 */

#include "pwmx.h"

static void _pwm_clk_enable(TIM_TypeDef *tim)
{
    if (tim == TIM1)
        __HAL_RCC_TIM1_CLK_ENABLE();
    else if (tim == TIM2)
        __HAL_RCC_TIM2_CLK_ENABLE();
    else if (tim == TIM3)
        __HAL_RCC_TIM3_CLK_ENABLE();
    else if (tim == TIM4)
        __HAL_RCC_TIM4_CLK_ENABLE();
    // thêm nếu chip có nhiều timer
}

void PwmX_Init(PwmX_t *pwm, TIM_TypeDef *tim, uint32_t channel, uint32_t freq_hz, uint32_t duty_pct)
{
    pwm->tim = tim;
    pwm->channel = channel;
    pwm->freq_hz = freq_hz;
    pwm->duty_pct = duty_pct;

    _pwm_clk_enable(tim);

    // cấu hình timer
    TIM_HandleTypeDef htim = {0};
    htim.Instance = tim;
    uint32_t clk = HAL_RCC_GetPCLK1Freq();
    uint32_t prescaler = 0;
    uint32_t period = (clk / freq_hz) - 1;
    htim.Init.Prescaler = prescaler;
    htim.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim.Init.Period = period;
    htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&htim);

    // cấu hình PWM channel
    TIM_OC_InitTypeDef sConfig = {0};
    sConfig.OCMode = TIM_OCMODE_PWM1;
    sConfig.Pulse = (period + 1) * duty_pct / 100;
    sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfig.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(&htim, &sConfig, channel);

    // Lưu handle vào static (nếu cần sau này chỉnh duty)
    __HAL_TIM_SET_COMPARE(&htim, channel, sConfig.Pulse);

    // Start ngay
    HAL_TIM_PWM_Start(&htim, channel);
}

void PwmX_Start(PwmX_t *pwm)
{
    TIM_HandleTypeDef htim = {.Instance = pwm->tim};
    HAL_TIM_PWM_Start(&htim, pwm->channel);
}

void PwmX_Stop(PwmX_t *pwm)
{
    TIM_HandleTypeDef htim = {.Instance = pwm->tim};
    HAL_TIM_PWM_Stop(&htim, pwm->channel);
}

void PwmX_SetDuty(PwmX_t *pwm, uint32_t duty_pct)
{
    pwm->duty_pct = duty_pct;
    uint32_t period = pwm->tim->ARR; // auto-reload value
    uint32_t pulse = (period + 1) * duty_pct / 100;
    __HAL_TIM_SET_COMPARE(&(TIM_HandleTypeDef){.Instance = pwm->tim}, pwm->channel, pulse);
}
