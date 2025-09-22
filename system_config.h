/*
 *	botton.h
 *	Created on: Sep 21, 2025
 *		Author: Trần Minh Đức
 */

#ifndef _SYSTEM_CONFIG_H_
#define _SYSTEM_CONFIG_H_

/* Button Default Config */
#define DEFAULT_PERIOD_MS                           (100)  // Thời gian 1 chu kỳ ON-OFF
#define DEFAULT_DUTY_PERCENT                        (50)   // Tỷ lệ thời gian ON-OFF

/* Button Default Config */
#define DEFAULT_DEBOUNCE_TIME_MS                    (30)   // lọc dội 20–50 ms là hợp lý
#define DEFAULT_CLICK_MAX_TIME_MS                   (250)  // max thời gian 1 click
#define DEFAULT_DOUBLE_CLICK_GAP_MS                 (350)  // khoảng chờ click thứ 2
#define DEFAULT_LONG_PRESS_TIME_MS                  (800)  // ngưỡng nhấn giữ lâu
#define DEFAULT_HOLD_REPEAT_INTERVAL_MS             (150)  // chu kỳ lặp khi giữ


#endif /* _SYSTEM_CONFIG_H_ */