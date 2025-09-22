/*
 *	led.c
 *	Created on: Sep 21, 2025
 *		Author: Trần Minh Đức
 */

#include "led.h"
#include "system_config.h"

void LedX_Init(LedX_t *led, GpioX_t io)
{
    led->io = io;
    GpioX_OutInit(io, GPIO_NOPULL, 0, GPIO_SPEED_FREQ_LOW);
    led->period_ms = DEFAULT_PERIOD_MS;
    led->duty_pct = DEFAULT_DUTY_PERCENT;
    led->tmark = AppTime_Millis();
    led->state = false;
    GpioX_Write(io, false);
}

void LedX_SetBlink(LedX_t *led, uint32_t period_ms, uint32_t duty_pct)
{
    led->period_ms = (period_ms == 0) ? 1 : period_ms;
    if (duty_pct > 100)
        duty_pct = 100;
    led->duty_pct = duty_pct;
}

void LedX_Run(LedX_t *led)
{
    uint32_t elapsed = (uint32_t)(AppTime_Millis() - led->tmark);
    uint32_t on_ms = (led->period_ms * led->duty_pct) / 100;
    if (!led->state)
    {
        if (elapsed >= led->period_ms - on_ms)
        {
            led->state = true;
            led->tmark = AppTime_Millis();
            GpioX_Write(led->io, true);
        }
    }
    else
    {
        if (elapsed >= on_ms)
        {
            led->state = false;
            led->tmark = AppTime_Millis();
            GpioX_Write(led->io, false);
        }
    }
}

void LedX_BlinkBlocking(GpioX_t io, uint32_t on_ms, uint32_t off_ms, uint32_t cycles)
{
    for (uint32_t i = 0; i < cycles; ++i)
    {
        GpioX_Write(io, true);
        AppTime_DelayMs(on_ms);
        GpioX_Write(io, false);
        AppTime_DelayMs(off_ms);
    }
}
