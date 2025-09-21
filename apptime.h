/*
 *	apptime.h
 *	Created on: Sep 21, 2025
 *		Author: Trần Minh Đức
 */

#ifndef _APP_TIME_H_
#define _APP_TIME_H_ 

// #include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

void AppTime_Init(void);
uint32_t AppTime_Millis(void);
uint64_t AppTime_Micros(void);
void AppTime_DelayMs(uint32_t ms);
void AppTime_DelayUs(uint32_t us);

static inline bool AppTime_Elapsed(uint32_t *t0, uint32_t interval_ms)
{
	uint32_t now = AppTime_Millis();
	if ((uint32_t)(now - *t0) >= interval_ms)
	{
		*t0 = now;
		return true;
	}
	return false;
}

#endif /* _APP_TIME_H_ */