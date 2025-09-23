/*
 *	led.h
 *	Created on: Sep 21, 2025
 *		Author: Trần Minh Đức
 */

#ifndef _LED_H_
#define _LED_H_

#include "gpio.h"
#include "apptime.h"

typedef struct
{
    GpioX_t io;
    uint32_t period_ms; // chu kỳ nháy
    uint32_t duty_pct;  // 0..100
    uint32_t tmark;     // timestamp cho non-blocking
    bool state;
} LedX_t;

void LedX_Init(LedX_t *led, GpioX_t io);
void LedX_SetBlink(LedX_t *led, uint32_t period_ms, uint32_t duty_pct);

void LedX_Run(LedX_t *led);

void LedX_BlinkBlocking(GpioX_t io, uint32_t on_ms, uint32_t off_ms, uint32_t cycles);

#endif /* _LED_H_ */
