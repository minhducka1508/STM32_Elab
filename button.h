/*
 *	botton.h
 *	Created on: Sep 21, 2025
 *		Author: Trần Minh Đức
 */

#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "gpio.h"
#include "apptime.h"

typedef enum
{
    BTN_EVT_NONE = 0,
    BTN_EVT_PRESS,
    BTN_EVT_RELEASE,
    BTN_EVT_CLICK,
    BTN_EVT_DOUBLE,
    BTN_EVT_LONG,
    BTN_EVT_HOLD_REPEAT
} BtnEvent_t;

typedef struct
{
    GpioX_t     io;
    bool        active_high;
    //Timing config (ms)
    uint16_t    debounce_ms;
    uint16_t    click_ms;
    uint16_t    double_ms;
    uint16_t    long_ms;
    uint16_t    hold_repeat_ms;
    //state
    uint32_t    t_change;
    uint32_t    t_press;
    uint32_t    t_last_release;
    bool        raw;
    bool        stable;
    bool        last_stable;
    uint8_t     click_counter;
    uint32_t    t_holp_repeat; // mốc phát sự kiện hold repeat lần cuối
} ButtonX_t;

void ButtonX_Init(ButtonX_t* b, GpioX_t io, bool active_high);
BtnEvent_t ButtonX_Update(ButtonX_t* b);
bool ButtonX_IsPressed(const ButtonX_t* b);

#endif /* _BUTTON_H_ */