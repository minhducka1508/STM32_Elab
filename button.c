/*
 *	botton.c
 *	Created on: Sep 21, 2025
 *		Author: Trần Minh Đức
 */

#include "button.h"
#include "system_config.h"
/*
Tiếp tục chỉnh sửa
*/
static bool read_raw(ButtonX_t* b)
{
    bool level = GpioX_Read(b->io);
    return b->active_high ? level : !level;
}

void ButtonX_Init(ButtonX_t* b, GpioX_t io, bool active_high)
{
    b->io = io;
    b->active_high = active_high;
    GpioX_InInit(io, active_high ? GPIO_PULLDOWN : GPIO_PULLUP);
    
    /* Default, Setting in System Config */
    b->debounce_ms = DEFAULT_DEBOUNCE_TIME_MS;
    b->click_ms = DEFAULT_CLICK_MAX_TIME_MS;
    b->double_ms = DEFAULT_DOUBLE_CLICK_GAP_MS;
    b->long_ms = DEFAULT_LONG_PRESS_TIME_MS;
    b->hold_repeat_ms = DEFAULT_HOLD_REPEAT_INTERVAL_MS;
    
    b->t_change = b->t_press = b->t_last_release = AppTime_Millis();
    b->raw = b->stable = b->last_stable = false;
    b->click_counter = 0;
    b->t_holp_repeat = 0;
}

bool ButtonX_IsPressed(const ButtonX_t* b)
{
    return b->stable;
}

BtnEvent_t ButtonX_Update(ButtonX_t* b)
{
    bool raw_present = read_raw(b);
    uint32_t time_now = AppTime_Millis();
    BtnEvent_t button_event = BTN_EVT_NONE;

    if (b->raw != raw_present)
    {
        b->raw = raw_present;
        b->t_change = time_now;
    }

    if ((uint32_t)(time_now - b->t_change) >= b->debounce_ms)
    {
        if (b->stable != b->raw)
        {
            b->last_stable = b->stable;
            b->stable = b->raw;

            if (b->stable)
            {
                b->t_press = time_now;
                b->t_holp_repeat = time_now;
                button_event = BTN_EVT_PRESS;
            }
            else
            {
                uint32_t press_duration = (uint32_t)(time_now - b->t_press);
                button_event = BTN_EVT_RELEASE;

                if (press_duration < b->click_ms)
                {
                    b->click_counter ++;
                    b->t_last_release = time_now;
                }
                else if (press_duration > b->long_ms)
                {
                    button_event = BTN_EVT_LONG;
                    b->click_counter = 0;
                }
                else
                {
                    b->click_counter ++;
                    b->t_last_release = time_now;
                }
            }
        }
        else
        {
            if (!b->stable && b->click_counter > 0)
            {
                if ((uint32_t)(time_now - b->t_last_release) >= b->double_ms)
                {
                    button_event = (b->click_counter >= 2) ? BTN_EVT_DOUBLE : BTN_EVT_CLICK;
                    b->click_counter = 0;
                }
            }
            if (b->stable)
            {
                uint32_t held_time = (uint32_t)(time_now - b->t_press);
                if (held_time >= b->long_ms)
                {
                    if ((uint32_t)(time_now - b->t_holp_repeat) >= b->hold_repeat_ms)
                    {
                        b->t_holp_repeat = time_now;
                        button_event = BTN_EVT_HOLD_REPEAT;
                    }
                }
            }
        }
    }
    return button_event;
}