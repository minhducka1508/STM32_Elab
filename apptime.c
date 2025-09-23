/*
 * 	apptime.c
 * 	Created on: Sep 21, 2025
 * 		Author: Trần Minh Đức
 */

#include "apptime.h"
// #include "stm32f1xx_hal.h"

// --- chọn Timer dùng cho AppTime ---
#define APTIME_TIMx 		TIM2
#define APTIME_IRQn 		TIM2_IRQn

static volatile uint32_t s_ms = 0; // đếm mili giây

// Khai báo handle cho HAL
static TIM_HandleTypeDef s_tim;

void AppTime_Init(void)
{
	// Bật clock cho TIM2 (hoặc TIM bạn chọn)
	__HAL_RCC_TIM2_CLK_ENABLE();

	// Cấu hình TIM2 chạy 1 MHz (1 tick = 1 µs)
	s_tim.Instance = APTIME_TIMx;
	s_tim.Init.Prescaler = (HAL_RCC_GetPCLK1Freq() / 1000000) - 1; // 1 MHz
	s_tim.Init.CounterMode = TIM_COUNTERMODE_UP;
	s_tim.Init.Period = 1000 - 1; // tràn mỗi 1000 µs = 1 ms
	s_tim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_Base_Init(&s_tim);

	// Enable interrupt
	__HAL_TIM_CLEAR_FLAG(&s_tim, TIM_FLAG_UPDATE);
	__HAL_TIM_ENABLE_IT(&s_tim, TIM_IT_UPDATE);

	// Start timer
	HAL_TIM_Base_Start_IT(&s_tim);

	// Enable NVIC
	HAL_NVIC_SetPriority(APTIME_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(APTIME_IRQn);
}

// --- ISR ---
void TIM2_IRQHandler(void)
{
	if (__HAL_TIM_GET_FLAG(&s_tim, TIM_FLAG_UPDATE) != RESET)
	{
		if (__HAL_TIM_GET_IT_SOURCE(&s_tim, TIM_IT_UPDATE) != RESET)
		{
			__HAL_TIM_CLEAR_IT(&s_tim, TIM_IT_UPDATE);
			s_ms++; // tăng biến mili giây
		}
	}
}

// --- API ---
uint32_t AppTime_Millis(void)
{
	return s_ms;
}

uint64_t AppTime_Micros(void)
{
	// Giá trị microsecond = mili giây đã đếm * 1000 + CNT hiện tại
	return (uint64_t)s_ms * 1000ULL + (uint64_t)(__HAL_TIM_GET_COUNTER(&s_tim));
}

void AppTime_DelayMs(uint32_t ms)
{
	uint32_t t0 = AppTime_Millis();
	while ((AppTime_Millis() - t0) < ms)
	{
		// spin
	}
}

void AppTime_DelayUs(uint32_t us)
{
	uint64_t t0 = AppTime_Micros();
	while ((AppTime_Micros() - t0) < us)
	{
		// spin
	}
}
