#pragma once

#include "wirish.h"
#include "drivers/board/systick.h"
#include "drivers/board/gpio.h"

#ifdef __cplusplus
extern "C"
{
#endif

    extern uint32_t CPU_FREQ;

#ifndef F_CPU
#define F_CPU CPU_FREQ
#endif

    //
    // Interrupts
    //
    static inline void interrupts()
    {
        asm volatile("cpsie i");
    }

    static inline void noInterrupts()
    {
        asm volatile("cpsid i");
    }

    //#define sei() interrupts
    //#define cli() noInterrupts()

    extern void setup();
    extern void loop();

    //TODO: int map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max);
    //TODO: void attachInterrupt(uint32_t pin, void (*callback)(), uint32_t mode);
    //TODO: void detachInterrupt(uint32_t pin);

#ifdef __cplusplus
}
#endif
