/*
 *	gpiox.c
 *	Created on: Sep 21, 2025
 *		Author: Trần Minh Đức
 */

#include "gpiox.h"

static void _gpio_clk_enable(GPIO_TypeDef *port)
{
	if (port == GPIOA)
		__HAL_RCC_GPIOA_CLK_ENABLE();
	else if (port == GPIOB)
		__HAL_RCC_GPIOB_CLK_ENABLE();
	else if (port == GPIOC)
		__HAL_RCC_GPIOC_CLK_ENABLE();
	else if (port == GPIOD)
		__HAL_RCC_GPIOD_CLK_ENABLE();
	else if (port == GPIOE)
		__HAL_RCC_GPIOE_CLK_ENABLE();
}

void GpioX_OutInit(GpioX_t io, uint32_t pull, uint32_t otype, uint32_t speed)
{
	_gpio_clk_enable(io.port);
	GPIO_InitTypeDef i = {0};
	i.Pin = io.pin;
	i.Mode = otype;
	i.Pull = pull;
	i.Speed = speed;
	HAL_GPIO_Init(io.port, &i);
}

void GpioX_InInit(GpioX_t io, uint32_t pull)
{
	_gpio_clk_enable(io.port);
	GPIO_InitTypeDef i = {0};
	i.Pin = io.pin;
	i.Mode = GPIO_MODE_INPUT;
	i.Pull = pull;
	HAL_GPIO_Init(io.port, &i);
}

void GpioX_Write(GpioX_t io, bool high)
{
	HAL_GPIO_WritePin(io.port, io.pin, high ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void GpioX_Toggle(GpioX_t io)
{
	HAL_GPIO_TogglePin(io.port, io.pin);
}

bool GpioX_Read(GpioX_t io)
{
	return (HAL_GPIO_ReadPin(io.port, io.pin) == GPIO_PIN_SET);
}
