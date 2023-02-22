#ifndef _BSP_H
#define _BSP_H

#include "string.h"
// #include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <inttypes.h>

#include "hc32_ddl.h"
#include "../core/macros.h"

#include "board_tim0.h"
#include "board_cfg.h"
#include "board_gpio.h"
#include "adc.h"
#include "../pins/hc32f46x/pins_Aquila_X2.h"
#include "../cores/usart.h"

#include "../cores/libmaple_types.h"
#include "../cores/wirish_types.h"

#ifdef __cplusplus
extern "C"
{
#endif

    extern uint32_t CPU_FREQ;

#ifndef F_CPU
#define F_CPU CPU_FREQ
#endif

    static inline void nvic_globalirq_enable()
    {
        asm volatile("cpsie i");
    }

    static inline void nvic_globalirq_disable()
    {
        asm volatile("cpsid i");
    }

    static inline void interrupts()
    {
        nvic_globalirq_enable();
    }

    static inline void noInterrupts()
    {
        nvic_globalirq_disable();
    }

    void init(void);
#ifdef __cplusplus
} // extern "C"
#endif

#endif
