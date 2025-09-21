/*
 *	apptime.c
 *	Created on: Sep 21, 2025
 *		Author: Trần Minh Đức
 */

#include "apptime.h"
#include "board_pin.h"

#if APTIME_USE_DWT
#define DWT_CTRL (*(volatile uint32_t *)0xE0001000)
#define DEMCR (*(volatile uint32_t *)0xE000EDFC)
#define DWT_CYCCNT (*(volatile uint32_t *)0xE0001004)
#endif

static volatile uint32_t s_ms = 0;

void AppTime_Init(void)
{
	// SysTick đã do HAL_Init() cấu hình -> HAL_IncTick() tăng 1ms
	// Ta đồng bộ biến s_ms bằng HAL_GetTick()
	s_ms = HAL_GetTick();

#if APTIME_USE_DWT
	// Enable DWT cycle counter (Cortex-M3/M4/M7)
	DEMCR |= (1 << 24);
	DWT_CYCCNT = 0;
	DWT_CTRL |= 1;
#else
	// Fallback: TIM2 chạy 1 MHz
	__HAL_RCC_TIM2_CLK_ENABLE(); // đổi nếu bạn chọn TIM khác
	TIM_HandleTypeDef htim = {0};
	htim.Instance = APTIME_TIMx;
	htim.Init.Prescaler = (HAL_RCC_GetHCLKFreq() / 1000000) - 1; // 1 MHz
	htim.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim.Init.Period = 0xFFFFFFFF;
	htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_Base_Init(&htim);
	HAL_TIM_Base_Start(&htim);
#endif
}

// HAL gọi mỗi 1ms: bảo đảm s_ms theo HAL tick (nếu muốn độc lập SysTick, có thể tự TIM)
void HAL_IncTick(void); // forward
void AppTime_TickHook_1ms(void) __attribute__((weak));
void AppTime_TickHook_1ms(void) {} // user hook

// Gọi trong SysTick_Handler sau HAL_IncTick(), nhưng HAL đã làm sẵn.
// Nếu muốn chủ động: có thể trong main loop đồng bộ s_ms = HAL_GetTick();
uint32_t AppTime_Millis(void) { return HAL_GetTick(); }

uint64_t AppTime_Micros(void)
{
#if APTIME_USE_DWT
	uint32_t cyccnt = DWT_CYCCNT;
	uint32_t cpu_hz = HAL_RCC_GetHCLKFreq();
	return ((uint64_t)cyccnt * 1000000ull) / cpu_hz;
#else
	return (uint64_t)__HAL_TIM_GET_COUNTER(&((TIM_TypeDef *)APTIME_TIMx));
#endif
}

void AppTime_DelayMs(uint32_t ms)
{
	uint32_t t0 = AppTime_Millis();
	while ((uint32_t)(AppTime_Millis() - t0) < ms)
	{ /* spin */
	}
}

void AppTime_DelayUs(uint32_t us)
{
	uint64_t t0 = AppTime_Micros();
	while ((AppTime_Micros() - t0) < us)
	{ /* spin */
	}
}
