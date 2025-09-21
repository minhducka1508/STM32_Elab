/*
 *	gpiox.h
 *	Created on: Sep 21, 2025
 *		Author: Trần Minh Đức
 */

#ifndef _GPIOX_H_
#define _GPIOX_H_

// #include "stm32f4xx_hal.h"
#include <stdbool.h>

typedef struct
{
	GPIO_TypeDef *port;
	uint16_t pin;
} GpioX_t;

void GpioX_OutInit(GpioX_t io, uint32_t pull, uint32_t otype, uint32_t speed);
void GpioX_InInit(GpioX_t io, uint32_t pull);
void GpioX_Write(GpioX_t io, bool high);
void GpioX_Toggle(GpioX_t io);
bool GpioX_Read(GpioX_t io);

#endif /* _GPIOX_H_ */