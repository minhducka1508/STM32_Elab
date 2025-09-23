/*
 *	exti.c
 *	Created on: Sep 23, 2025
 *		Author: Trần Minh Đức
 */

#include "gpio.h"
#include "exti.h"
#include "system_config.h"

static ExtiCallback_t exti_callback[MAX_EXTI_LINES];

void Exti_Init(GpioX_t exti_io, ExtiTrigger_t trigger, ExtiCallback_t fuc_cb, uint32_t active_high)
{
    uint32_t pin_number = __builtin_ctz(exti_io.pin);
    GpioX_InInit(exti_io, active_high); // Enable clock GPIO
    /* Enable SYSCFG clock để map port -> EXIT line */
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    uint8_t port_index = ((uint32_t)exti_io.port - (uint32_t)GPIOA) / 0x400;
    SYSCFG->EXTICR[pin_number >> 2] &= ~(0xF << (4 * (pin_number & 0x3)));
    SYSCFG->EXTICR[pin_number >> 2] |= (port_index << (4 * (pin_number & 0x3)));

    // 3. Cấu hình trigger
    if (trigger == EXTI_RISING || trigger == EXTI_RISING_FALLING)
        EXTI->RTSR |= (1 << pin_number);
    if (trigger == EXTI_FALLING || trigger == EXTI_RISING_FALLING)
        EXTI->FTSR |= (1 << pin_number);

    // 4. Enable mask
    EXTI->IMR |= (1 << pin_number);

    // 5. Lưu callback
    exti_callback[pin_number] = fuc_cb;

    // 6. Enable NVIC IRQ
    if (pin_number <= 4)
        NVIC_EnableIRQ((IRQn_Type)(EXTI0_IRQn + pin_number));
    else if (pin_number <= 9)
        NVIC_EnableIRQ(EXTI9_5_IRQn);
    else
        NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void Exti_IRQHandler(uint16_t pin)
{
    uint32_t pin_number = __builtin_ctz(pin);
    if (EXTI->PR & (1 << pin_number))
    {
        EXTI->PR = (1 << pin_number); // clear pending
        if (exti_callback[pin_number])
        {
            exti_callback[pin_number](); // gọi callback user
        }
    }
}

/* ---------------- IRQ Handlers thực tế ---------------- */

void EXTI0_IRQHandler(void) { Exti_IRQHandler(GPIO_PIN_0); }
void EXTI1_IRQHandler(void) { Exti_IRQHandler(GPIO_PIN_1); }
void EXTI2_IRQHandler(void) { Exti_IRQHandler(GPIO_PIN_2); }
void EXTI3_IRQHandler(void) { Exti_IRQHandler(GPIO_PIN_3); }
void EXTI4_IRQHandler(void) { Exti_IRQHandler(GPIO_PIN_4); }

void EXTI9_5_IRQHandler(void)
{
    for (uint16_t pin = GPIO_PIN_5; pin <= GPIO_PIN_9; pin <<= 1)
        Exti_IRQHandler(pin);
}

void EXTI15_10_IRQHandler(void)
{
    for (uint16_t pin = GPIO_PIN_10; pin <= GPIO_PIN_15; pin <<= 1)
        Exti_IRQHandler(pin);
}